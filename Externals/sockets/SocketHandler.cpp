/** \file SocketHandler.cpp
 **	\date  2004-02-13
 **	\author grymse@alhem.net
**/
/*
Copyright (C) 2004-2011  Anders Hedstrom

This library is made available under the terms of the GNU GPL, with
the additional exemption that compiling, linking, and/or using OpenSSL
is allowed.

If you would like to use this library in a closed-source application,
a separate license agreement is available. For information about
the closed-source license agreement for the C++ sockets library,
please visit http://www.alhem.net/Sockets/license.html and/or
email license@alhem.net.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/
#ifdef _WIN32
#ifdef _MSC_VER
#pragma warning(disable:4786)
#endif
#endif
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>

#include "SocketHandler.h"
#include "UdpSocket.h"
#include "TcpSocket.h"
#include "IMutex.h"
#include "Utility.h"
#include "SocketAddress.h"
#include "Exception.h"
#include "SocketHandlerThread.h"
#include "Lock.h"

#ifdef SOCKETS_NAMESPACE
namespace SOCKETS_NAMESPACE {
#endif

#ifdef _DEBUG
#define DEB(x) x; fflush(stderr);
#else
#define DEB(x) 
#endif


SocketHandler::SocketHandler(StdLog *p)
    :m_stdlog(p)
    , m_mutex(m_mutex)
    , m_b_use_mutex(false)
    , m_parent(m_parent)
    , m_b_parent_is_valid(false)
    , m_release(NULL)
    , m_maxsock(0)
    , m_tlast(0)
    , m_b_check_callonconnect(false)
    , m_b_check_detach(false)
    , m_b_check_timeout(false)
    , m_b_check_retry(false)
    , m_b_check_close(false)
#ifdef ENABLE_POOL
    , m_b_enable_pool(false)
#endif
#ifdef ENABLE_DETACH
    , m_slave(false)
#endif
{
    FD_ZERO(&m_rfds);
    FD_ZERO(&m_wfds);
    FD_ZERO(&m_efds);
}


SocketHandler::SocketHandler(IMutex& mutex, StdLog *p)
    :m_stdlog(p)
    , m_mutex(mutex)
    , m_b_use_mutex(true)
    , m_parent(m_parent)
    , m_b_parent_is_valid(false)
    , m_release(NULL)
    , m_maxsock(0)
    , m_tlast(0)
    , m_b_check_callonconnect(false)
    , m_b_check_detach(false)
    , m_b_check_timeout(false)
    , m_b_check_retry(false)
    , m_b_check_close(false)
#ifdef ENABLE_POOL
    , m_b_enable_pool(false)
#endif
#ifdef ENABLE_DETACH
    , m_slave(false)
#endif
{
    m_mutex.Lock();
    FD_ZERO(&m_rfds);
    FD_ZERO(&m_wfds);
    FD_ZERO(&m_efds);
}


SocketHandler::SocketHandler(IMutex& mutex, ISocketHandler& parent, StdLog *p)
    :m_stdlog(p)
    , m_mutex(mutex)
    , m_b_use_mutex(true)
    , m_parent(parent)
    , m_b_parent_is_valid(true)
    , m_release(NULL)
    , m_maxsock(0)
    , m_tlast(0)
    , m_b_check_callonconnect(false)
    , m_b_check_detach(false)
    , m_b_check_timeout(false)
    , m_b_check_retry(false)
    , m_b_check_close(false)
#ifdef ENABLE_POOL
    , m_b_enable_pool(false)
#endif
#ifdef ENABLE_DETACH
    , m_slave(false)
#endif
{
    m_mutex.Lock();
    FD_ZERO(&m_rfds);
    FD_ZERO(&m_wfds);
    FD_ZERO(&m_efds);
}


SocketHandler::~SocketHandler()
{
    for (std::list<SocketHandlerThread *>::iterator it = m_threads.begin(); it != m_threads.end(); ++it)
    {
        SocketHandlerThread *p = *it;
        p->Stop();
    }
    {
        while (m_sockets.size())
        {
            DEB(fprintf(stderr, "Emptying sockets list in SocketHandler destructor, %d instances\n", (int)m_sockets.size());)
                socket_m::iterator it = m_sockets.begin();
            std::shared_ptr<Socket> p = it->second;
            if (p)
            {
                DEB(fprintf(stderr, "  fd %d\n", p->GetSocket());)
                    p->Close();
                DEB(fprintf(stderr, "  fd closed %d\n", p->GetSocket());)
                    //				p -> OnDelete(); // hey, I turn this back on. what's the worst that could happen??!!
                                    // MinionSocket breaks, calling MinderHandler methods in OnDelete -
                                    // MinderHandler is already gone when that happens...

                                    // only delete socket when controlled
                                    // ie master sockethandler can delete non-detached sockets
                                    // and a slave sockethandler can only delete a detach socket
                    if (p->DeleteByHandler()
#ifdef ENABLE_DETACH
                        && !(m_slave ^ p->IsDetached())
#endif
                        )
                    {
                        p->SetErasedByHandler();
                        p.reset();
                    }
                m_sockets.erase(it);
            }
            else
            {
                m_sockets.erase(it);
            }
            DEB(fprintf(stderr, "next\n");)
        }
        DEB(fprintf(stderr, "/Emptying sockets list in SocketHandler destructor, %d instances\n", (int)m_sockets.size());)
    }
    if (m_b_use_mutex)
    {
        m_mutex.Unlock();
    }
}


ISocketHandler *SocketHandler::Create(StdLog *log)
{
    return new SocketHandler(log);
}


ISocketHandler *SocketHandler::Create(IMutex& mutex, ISocketHandler& parent, StdLog *log)
{
    return new SocketHandler(mutex, parent, log);
}


bool SocketHandler::ParentHandlerIsValid()
{
    return m_b_parent_is_valid;
}


ISocketHandler& SocketHandler::ParentHandler()
{
    if (!m_b_parent_is_valid)
        throw Exception("No parent sockethandler available");
    return m_parent;
}


ISocketHandler& SocketHandler::GetRandomHandler()
{
    if (m_threads.empty())
        throw Exception("SocketHandler is not multithreaded");
    size_t min_count = 99999;
    SocketHandlerThread *match = NULL;
    for (std::list<SocketHandlerThread *>::iterator it = m_threads.begin(); it != m_threads.end(); ++it)
    {
        SocketHandlerThread *thr = *it;
        ISocketHandler& h = thr->Handler();
        {
            Lock lock(h.GetMutex());
            size_t sz = h.GetCount();
            if (sz < min_count)
            {
                min_count = sz;
                match = thr;
            }
        }
    }
    if (match)
        return match->Handler();
    throw Exception("Can't locate free threaded sockethandler");
}


ISocketHandler& SocketHandler::GetEffectiveHandler()
{
    return m_b_parent_is_valid ? m_parent : *this;
}


void SocketHandler::SetNumberOfThreads(size_t n)
{
    if (!m_threads.empty())
    {
        return; // already set
    }
    if (n > 1 && n < 256)
    {
        for (int i = 1; i <= (int)n; i++)
        {
            SocketHandlerThread *p = new SocketHandlerThread(*this);
            m_threads.push_back(p);
            p->SetDeleteOnExit();
            p->Start();
            p->Wait();
        }
    }
}


bool SocketHandler::IsThreaded()
{
    return !m_threads.empty();
}


void SocketHandler::EnableRelease()
{
    if (m_release)
        return;

    m_release = std::make_shared<UdpSocket>(*this);
    m_release->SetDeleteByHandler();
    port_t port = 0;
    m_release->Bind("127.0.0.1", port);
    Add(m_release);
}


void SocketHandler::Release()
{
    if (!m_release)
        return;
    m_release->SendTo("127.0.0.1", m_release->GetPort(), "\n");
}


IMutex& SocketHandler::GetMutex() const
{
    return m_mutex;
}


#ifdef ENABLE_DETACH
void SocketHandler::SetSlave(bool x)
{
    m_slave = x;
}


bool SocketHandler::IsSlave()
{
    return m_slave;
}
#endif


void SocketHandler::RegStdLog(StdLog *log)
{
    m_stdlog = log;
}


void SocketHandler::LogError(std::shared_ptr<Socket> p, const std::string& user_text, int err, const std::string& sys_err, loglevel_t t)
{
    if (m_stdlog)
    {
        m_stdlog->error(this, p, user_text, err, sys_err, t);
    }
}


void SocketHandler::Add(std::shared_ptr<Socket> p)
{
    m_add.push_back(p); // no checks here
}


void SocketHandler::ISocketHandler_Add(std::shared_ptr<Socket> p, bool bRead, bool bWrite)
{
    Set(p, bRead, bWrite);
}


void SocketHandler::ISocketHandler_Mod(std::shared_ptr<Socket> p, bool bRead, bool bWrite)
{
    Set(p, bRead, bWrite);
}


void SocketHandler::ISocketHandler_Del(std::shared_ptr<Socket> p)
{
    Set(p, false, false);
}


void SocketHandler::Set(std::shared_ptr<Socket> p, bool bRead, bool bWrite)
{
    SOCKET s = p->GetSocket();
    if (s >= 0)
    {
        bool bException = true;
        if (bRead)
        {
            if (!FD_ISSET(s, &m_rfds))
            {
                FD_SET(s, &m_rfds);
            }
        }
        else
        {
            FD_CLR(s, &m_rfds);
        }
        if (bWrite)
        {
            if (!FD_ISSET(s, &m_wfds))
            {
                FD_SET(s, &m_wfds);
            }
        }
        else
        {
            FD_CLR(s, &m_wfds);
        }
        if (bException)
        {
            if (!FD_ISSET(s, &m_efds))
            {
                FD_SET(s, &m_efds);
            }
        }
        else
        {
            FD_CLR(s, &m_efds);
        }
    }
}

bool SocketHandler::Valid(std::shared_ptr<Socket> p0)
{
    for (socket_m::iterator it = m_sockets.begin(); it != m_sockets.end(); ++it)
    {
        std::shared_ptr<Socket> p = it->second;
        if (p0 == p)
            return true;
    }
    return false;
}


bool SocketHandler::Valid(socketuid_t uid)
{
    for (socket_m::iterator it = m_sockets.begin(); it != m_sockets.end(); ++it)
    {
        std::shared_ptr<Socket> p = it->second;
        if (p->UniqueIdentifier() == uid)
            return true;
    }
    return false;
}


bool SocketHandler::OkToAccept(std::shared_ptr<Socket>)
{
    return true;
}


size_t SocketHandler::GetCount()
{
    return m_sockets.size() + m_add.size() + m_delete.size();
}

#ifdef ENABLE_POOL
std::shared_ptr<ISocketHandler::PoolSocket> SocketHandler::FindConnection(int type, const std::string& protocol, SocketAddress& ad)
{
    for (socket_m::iterator it = m_sockets.begin(); it != m_sockets.end() && m_sockets.size(); ++it)
    {
        std::shared_ptr<PoolSocket> pools = std::dynamic_pointer_cast<PoolSocket>(it->second);
        if (pools)
        {
            if (pools->GetSocketType() == type &&
                pools->GetSocketProtocol() == protocol &&
                // %!			    pools -> GetClientRemoteAddress() &&
                *pools->GetClientRemoteAddress() == ad)
            {
                m_sockets.erase(it);
                pools->SetRetain(); // avoid Close in Socket destructor
                return pools; // Caller is responsible that this socket is deleted
            }
        }
    }
    return NULL;
}


void SocketHandler::EnablePool(bool x)
{
    m_b_enable_pool = x;
}


bool SocketHandler::PoolEnabled()
{
    return m_b_enable_pool;
}
#endif


void SocketHandler::Remove(std::shared_ptr<Socket> Sock)
{
    if (Sock->ErasedByHandler())
    {
        return;
    }

    for (socket_m::iterator it = m_sockets.begin(); it != m_sockets.end(); ++it)
    {
        if (it->second == Sock)
        {
            LogError(Sock, "Remove", -1, "Socket destructor called while still in use", LOG_LEVEL_WARNING);
            m_sockets.erase(it);
            return;
        }
    }

    for (std::list<std::shared_ptr<Socket>>::iterator it2 = m_add.begin(); it2 != m_add.end(); ++it2)
    {
        if (*it2 == Sock)
        {
            LogError(Sock, "Remove", -2, "Socket destructor called while still in use", LOG_LEVEL_WARNING);
            m_add.erase(it2);
            return;
        }
    }
    for (std::list<std::shared_ptr<Socket>>::iterator it3 = m_delete.begin(); it3 != m_delete.end(); ++it3)
    {
        if (*it3 == Sock)
        {
            LogError(Sock, "Remove", -3, "Socket destructor called while still in use", LOG_LEVEL_WARNING);
            m_delete.erase(it3);
            return;
        }
    }
}


void SocketHandler::SetCallOnConnect(bool x)
{
    m_b_check_callonconnect = x;
}


void SocketHandler::SetDetach(bool x)
{
    m_b_check_detach = x;
}


void SocketHandler::SetTimeout(bool x)
{
    m_b_check_timeout = x;
}


void SocketHandler::SetRetry(bool x)
{
    m_b_check_retry = x;
}


void SocketHandler::SetClose(bool x)
{
    m_b_check_close = x;
}


void SocketHandler::DeleteSocket(std::shared_ptr<Socket> p)
{
    p->OnDelete();
    if (p->DeleteByHandler() && !p->ErasedByHandler())
    {
        p->SetErasedByHandler();
    }
    m_fds_erase.push_back(p->UniqueIdentifier());
}


void SocketHandler::RebuildFdset()
{
    fd_set rfds;
    fd_set wfds;
    fd_set efds;
    // rebuild fd_set's from active sockets list (m_sockets) here
    FD_ZERO(&rfds);
    FD_ZERO(&wfds);
    FD_ZERO(&efds);
    for (socket_m::iterator it = m_sockets.begin(); it != m_sockets.end(); ++it)
    {
        SOCKET s = it->first;
        std::shared_ptr<Socket> p = it->second;
        if (s == p->GetSocket() && s >= 0)
        {
            fd_set fds;
            FD_ZERO(&fds);
            FD_SET(s, &fds);
            struct timeval tv;
            tv.tv_sec = 0;
            tv.tv_usec = 0;
            int n = select((int)s + 1, &fds, NULL, NULL, &tv);
            if (n == -1 && Errno == EBADF)
            {
                // %! bad fd, remove
                LogError(p, "Select", (int)s, "Bad fd in fd_set (2)", LOG_LEVEL_ERROR);
                if (Valid(p) && Valid(p->UniqueIdentifier()))
                {
                    DeleteSocket(p);
                }
            }
            else
            {
                if (FD_ISSET(s, &m_rfds))
                    FD_SET(s, &rfds);
                if (FD_ISSET(s, &m_wfds))
                    FD_SET(s, &wfds);
                if (FD_ISSET(s, &m_efds))
                    FD_SET(s, &efds);
            }
        }
        else
        {
            // %! mismatch
            LogError(p, "Select", (int)s, "Bad fd in fd_set (3)", LOG_LEVEL_ERROR);
            DeleteSocket(p);
        }
    }
    m_rfds = rfds;
    m_wfds = wfds;
    m_efds = efds;
}


void SocketHandler::AddIncoming()
{
    while (m_add.size() > 0)
    {
        if (m_sockets.size() >= MaxCount())
        {
            LogError(NULL, "Select", (int)m_sockets.size(), "socket limit reached", LOG_LEVEL_WARNING);
            break;
        }
        std::list<std::shared_ptr<Socket>>::iterator it = m_add.begin();
        std::shared_ptr<Socket> p = *it;
        SOCKET s = p->GetSocket();
        DEB(fprintf(stderr, "Trying to add fd %d,  m_add.size() %d\n", (int)s, (int)m_add.size());)
            //
            if (s == INVALID_SOCKET)
            {
                LogError(p, "Add", -1, "Invalid socket", LOG_LEVEL_WARNING);
                m_delete.push_back(p);
                m_add.erase(it);
                continue;
            }
        socket_m::iterator it2;
        if ((it2 = m_sockets.find(s)) != m_sockets.end())
        {
            std::shared_ptr<Socket> found = it2->second;
            if (p->UniqueIdentifier() > found->UniqueIdentifier())
            {
                LogError(p, "Add", (int)p->GetSocket(), "Replacing socket already in controlled queue (newer uid)", LOG_LEVEL_WARNING);
                // replace
                DeleteSocket(found);
            }
            else
                if (p->UniqueIdentifier() == found->UniqueIdentifier())
                {
                    LogError(p, "Add", (int)p->GetSocket(), "Attempt to add socket already in controlled queue (same uid)", LOG_LEVEL_ERROR);
                    // same - ignore
                    if (p != found)
                        m_delete.push_back(p);
                    m_add.erase(it);
                    continue;
                }
                else
                {
                    LogError(p, "Add", (int)p->GetSocket(), "Attempt to add socket already in controlled queue (older uid)", LOG_LEVEL_FATAL);
                    // %! it's a dup, don't add to delete queue, just ignore it
                    m_delete.push_back(p);
                    m_add.erase(it);
                    continue;
                }
        }
        if (p->CloseAndDelete())
        {
            LogError(p, "Add", (int)p->GetSocket(), "Added socket with SetCloseAndDelete() true", LOG_LEVEL_WARNING);
            m_sockets[s] = p;
            DeleteSocket(p);
            p->Close();
        }
        else
        {
            m_b_check_callonconnect |= p->CallOnConnect();
            m_b_check_detach |= p->IsDetach();
            m_b_check_timeout |= p->CheckTimeout();
            m_b_check_retry |= p->RetryClientConnect();
            /*
                        if (p -> CallOnConnect())
                        {
                            m_b_check_callonconnect = true;
                        }
                        if (p -> IsDetach())
                        {
                            m_b_check_detach = true;
                        }
                        if (p -> CheckTimeout())
                        {
                            m_b_check_timeout = true;
                        }
                        if (p -> RetryClientConnect())
                        {
                            m_b_check_retry = true;
                        }
            */
            std::shared_ptr<StreamSocket> scp = std::dynamic_pointer_cast<StreamSocket>(p);
            if (scp && scp->Connecting()) // 'Open' called before adding socket
            {
                ISocketHandler_Add(p, false, true);
            }
            else
            {
                std::shared_ptr<TcpSocket> tcp = std::dynamic_pointer_cast<TcpSocket>(p);
                bool bWrite = tcp ? tcp->GetOutputLength() != 0 : false;
                if (p->IsDisableRead())
                {
                    ISocketHandler_Add(p, false, bWrite);
                }
                else
                {
                    ISocketHandler_Add(p, true, bWrite);
                }
            }
            m_maxsock = (s > m_maxsock) ? s : m_maxsock;
            m_sockets[s] = p;
        }
        //
        m_add.erase(it);
    }
}


void SocketHandler::CheckErasedSockets()
{
    // check erased sockets
    bool check_max_fd = false;
    while (m_fds_erase.size())
    {
        std::list<socketuid_t>::iterator it = m_fds_erase.begin();
        socketuid_t uid = *it;
        for (socket_m::iterator it = m_sockets.begin(); it != m_sockets.end(); ++it)
        {
            std::shared_ptr<Socket> p = it->second;
            if (p->UniqueIdentifier() == uid)
            {
                /* Sometimes a SocketThread class can finish its run before the master
                   sockethandler gets here. In that case, the SocketThread has set the
                   'ErasedByHandler' flag on the socket which will make us end up with a
                   double delete on the socket instance.
                   The fix is to make sure that the master sockethandler only can delete
                   non-detached sockets, and a slave sockethandler only can delete
                   detach sockets. */
                if (p->ErasedByHandler()
#ifdef ENABLE_DETACH
                    && !(m_slave ^ p->IsDetached())
#endif
                    )
                {
                    p.reset();
                }
                m_sockets.erase(it);
                break;
            }
        }
        m_fds_erase.erase(it);
        check_max_fd = true;
    }
    // calculate max file descriptor for select() call
    if (check_max_fd)
    {
        m_maxsock = 0;
        for (socket_m::iterator it = m_sockets.begin(); it != m_sockets.end(); ++it)
        {
            SOCKET s = it->first;
            m_maxsock = s > m_maxsock ? s : m_maxsock;
        }
    }
}


void SocketHandler::CheckCallOnConnect()
{
    m_b_check_callonconnect = false;
    for (socket_m::iterator it = m_sockets.begin(); it != m_sockets.end(); ++it)
    {
        std::shared_ptr<Socket> p = it->second;
        if (Valid(p) && Valid(p->UniqueIdentifier()) && p->CallOnConnect())
        {
            p->SetConnected(); // moved here from inside if (tcp) check below
                {
                    std::shared_ptr<TcpSocket> tcp = std::dynamic_pointer_cast<TcpSocket>(p);
                    if (tcp)
                    {
                        if (tcp->GetOutputLength())
                        {
                            p->OnWrite();
                        }
                    }
#ifdef ENABLE_RECONNECT
                    if (tcp && tcp->IsReconnect())
                        p->OnReconnect();
                    else
#endif
                    {
                        p->OnConnect();
                    }
                }
            p->SetCallOnConnect(false);
            m_b_check_callonconnect = true;
        }
    }
}


#ifdef ENABLE_DETACH
void SocketHandler::CheckDetach()
{
    m_b_check_detach = false;
    for (socket_m::iterator it = m_sockets.begin(); it != m_sockets.end(); ++it)
    {
        std::shared_ptr<Socket> p = it->second;
        if (p->IsDetach())
        {
            ISocketHandler_Del(p);
            m_sockets.erase(it); // we don't want this around anymore
            // After DetachSocket(), all calls to Handler() will return a reference
            // to the new slave SocketHandler running in the new thread.
            p->DetachSocket();
            // Adding the file descriptor to m_fds_erase will now also remove the
            // socket from the detach queue - tnx knightmad
//			m_fds_erase.push_back(p -> UniqueIdentifier());
            m_b_check_detach = true;
            break; // 'it' is invalid
        }
    }
    for (std::list<std::shared_ptr<Socket>>::iterator it = m_add.begin(); it != m_add.end() && !m_b_check_detach; ++it)
    {
        std::shared_ptr<Socket> p = *it;
        m_b_check_detach |= p->IsDetach();
    }
}
#endif


void SocketHandler::CheckTimeout(time_t tnow)
{
    m_b_check_timeout = false;
    for (socket_m::iterator it = m_sockets.begin(); it != m_sockets.end(); ++it)
    {
        std::shared_ptr<Socket> p = it->second;
        if (Valid(p) && Valid(p->UniqueIdentifier()) && p->CheckTimeout())
        {
            if (p->Timeout(tnow))
            {
                std::shared_ptr<StreamSocket> scp = std::dynamic_pointer_cast<StreamSocket>(p);
                p->SetTimeout(0);
                if (scp && scp->Connecting())
                {
                    p->OnConnectTimeout();
                    // restart timer
                    p->SetTimeout(scp->GetConnectTimeout());
                }
                else
                {
                    p->OnTimeout();
                }
            }
            m_b_check_timeout = true;
        }
    }
}


void SocketHandler::CheckRetry()
{
    m_b_check_retry = false;
    for (socket_m::iterator it = m_sockets.begin(); it != m_sockets.end(); ++it)
    {
        std::shared_ptr<Socket> p = it->second;
        if (Valid(p) && Valid(p->UniqueIdentifier()) && p->RetryClientConnect())
        {
            std::shared_ptr<TcpSocket> tcp = std::dynamic_pointer_cast<TcpSocket>(p);
            tcp->SetRetryClientConnect(false);
            DEB(fprintf(stderr, "Close() before retry client connect\n");)
                p->Close(); // removes from m_fds_retry
            std::auto_ptr<SocketAddress> ad = p->GetClientRemoteAddress();
            if (ad.get())
            {
                tcp->Open(*ad);
            }
            else
            {
                LogError(p, "RetryClientConnect", 0, "no address", LOG_LEVEL_ERROR);
            }
            Add(p);
            m_fds_erase.push_back(p->UniqueIdentifier());
            m_b_check_retry = true;
        }
    }
}


void SocketHandler::CheckClose()
{
    m_b_check_close = false;
    for (socket_m::iterator it = m_sockets.begin(); it != m_sockets.end(); ++it)
    {
        std::shared_ptr<Socket> p = it->second;
        if (Valid(p) && Valid(p->UniqueIdentifier()) && p->CloseAndDelete())
        {
            std::shared_ptr<TcpSocket> tcp = std::dynamic_pointer_cast<TcpSocket>(p);
#ifdef ENABLE_RECONNECT
            if (p->Lost() && !(tcp && tcp->Reconnect()))
#else
            if (p->Lost())
#endif
            {
                // remove instance when Lost, if not reconnect flag is set
                DeleteSocket(p);
            }
            else
                // new graceful tcp - flush and close timeout 5s
                if (tcp && p->IsConnected() && tcp->GetFlushBeforeClose() && p->TimeSinceClose() < 5)
                {
                    DEB(fprintf(stderr, " close(1)\n");)
                        if (tcp->GetOutputLength())
                        {
                            LogError(p, "Closing", (int)tcp->GetOutputLength(), "Sending all data before closing", LOG_LEVEL_INFO);
                        }
                        else // shutdown write when output buffer is empty
                            if (!(tcp->GetShutdown() & SHUT_WR))
                            {
                                SOCKET nn = it->first;
                                if (nn != INVALID_SOCKET && shutdown(nn, SHUT_WR) == -1)
                                {
                                    LogError(p, "graceful shutdown", Errno, StrError(Errno), LOG_LEVEL_ERROR);
                                }
                                tcp->SetShutdown(SHUT_WR);
                            }
                            else
                            {
                                ISocketHandler_Del(p);
                                tcp->Close();
                                DeleteSocket(p);
                            }
                }
                else
#ifdef ENABLE_RECONNECT
                    if (tcp && p->IsConnected() && tcp->Reconnect())
                    {
                        p->SetCloseAndDelete(false);
                        tcp->SetIsReconnect();
                        p->SetConnected(false);
                        DEB(fprintf(stderr, "Close() before reconnect\n");)
                            p->Close(); // dispose of old file descriptor (Open creates a new)
                        p->OnDisconnect();
                        std::auto_ptr<SocketAddress> ad = p->GetClientRemoteAddress();
                        if (ad.get())
                        {
                            tcp->Open(*ad);
                        }
                        else
                        {
                            LogError(p, "Reconnect", 0, "no address", LOG_LEVEL_ERROR);
                        }
                        tcp->ResetConnectionRetries();
                        Add(p);
                        m_fds_erase.push_back(p->UniqueIdentifier());
                    }
                    else
#endif
                    {
                        if (tcp && p->IsConnected() && tcp->GetOutputLength())
                        {
                            LogError(p, "Closing", (int)tcp->GetOutputLength(), "Closing socket while data still left to send", LOG_LEVEL_WARNING);
                        }
#ifdef ENABLE_POOL
                        if (p->Retain() && !p->Lost())
                        {
                            std::shared_ptr<PoolSocket> p2 = std::make_shared<PoolSocket>(*this, p);
                            p2->SetDeleteByHandler();
                            Add(p2);
                            //
                            p->SetCloseAndDelete(false); // added - remove from m_fds_close
                        }
                        else
#endif // ENABLE_POOL
                        {
                            ISocketHandler_Del(p);
                            DEB(fprintf(stderr, "Close() before OnDelete\n");)
                                p->Close();
                        }
                        DeleteSocket(p);
                    }
            m_b_check_close = true;
        }
    }
}


int SocketHandler::ISocketHandler_Select(struct timeval *tsel)
{
#ifdef MACOSX
    fd_set rfds;
    fd_set wfds;
    fd_set efds;
    FD_COPY(&m_rfds, &rfds);
    FD_COPY(&m_wfds, &wfds);
    FD_COPY(&m_efds, &efds);
#else
    fd_set rfds = m_rfds;
    fd_set wfds = m_wfds;
    fd_set efds = m_efds;
#endif
    int n;
    DEB(
        printf("select( %d, [", m_maxsock + 1);
    for (size_t i = 0; i <= m_maxsock; i++)
        if (FD_ISSET(i, &rfds))
            printf(" %d", i);
    printf("], [");
    for (size_t i = 0; i <= m_maxsock; i++)
        if (FD_ISSET(i, &wfds))
            printf(" %d", i);
    printf("], [");
    for (size_t i = 0; i <= m_maxsock; i++)
        if (FD_ISSET(i, &efds))
            printf(" %d", i);
    printf("]\n");
    )
        if (m_b_use_mutex)
        {
            m_mutex.Unlock();
            n = select((int)(m_maxsock + 1), &rfds, &wfds, &efds, tsel);
            m_mutex.Lock();
        }
        else
        {
            n = select((int)(m_maxsock + 1), &rfds, &wfds, &efds, tsel);
        }
    if (n == -1) // error on select
    {
        int err = Errno;
        /*
            EBADF  An invalid file descriptor was given in one of the sets.
            EINTR  A non blocked signal was caught.
            EINVAL n is negative. Or struct timeval contains bad time values (<0).
            ENOMEM select was unable to allocate memory for internal tables.
        */
#ifdef _WIN32
        switch (err)
        {
        case WSAENOTSOCK:
            RebuildFdset();
            break;
        case WSAEINTR:
        case WSAEINPROGRESS:
            break;
        case WSAEINVAL:
            LogError(NULL, "SocketHandler::Select", err, StrError(err), LOG_LEVEL_FATAL);
            throw Exception("select(n): n is negative. Or struct timeval contains bad time values (<0).");
        case WSAEFAULT:
            LogError(NULL, "SocketHandler::Select", err, StrError(err), LOG_LEVEL_ERROR);
            break;
        case WSANOTINITIALISED:
            throw Exception("WSAStartup not successfully called");
        case WSAENETDOWN:
            throw Exception("Network subsystem failure");
        }
#else
        switch (err)
        {
        case EBADF:
            RebuildFdset();
            break;
        case EINTR:
            break;
        case EINVAL:
            LogError(NULL, "SocketHandler::Select", err, StrError(err), LOG_LEVEL_FATAL);
            throw Exception("select(n): n is negative. Or struct timeval contains bad time values (<0).");
        case ENOMEM:
            LogError(NULL, "SocketHandler::Select", err, StrError(err), LOG_LEVEL_ERROR);
            break;
        }
#endif
        printf("error on select(): %d %s\n", Errno, StrError(err));
    }
    else
        if (!n) // timeout
        {
        }
        else
            if (n > 0)
            {
                for (socket_m::iterator it = m_sockets.begin(); it != m_sockets.end(); ++it)
                {
                    SOCKET i = it->first;
                    std::shared_ptr<Socket> p = it->second;
                    // ---------------------------------------------------------------------------------
                    if (FD_ISSET(i, &rfds))
                    {
                        {
                            p->OnRead();
                        }
                    }
                    // ---------------------------------------------------------------------------------
                    if (FD_ISSET(i, &wfds))
                    {
                        {
                            p->OnWrite();
                        }
                    }
                    // ---------------------------------------------------------------------------------
                    if (FD_ISSET(i, &efds))
                    {
                        p->OnException();
                    }
                } // m_sockets ...
            } // if (n > 0)
    return n;
}


int SocketHandler::Select(long sec, long usec)
{
    struct timeval tv;
    tv.tv_sec = sec;
    tv.tv_usec = usec;
    return Select(&tv);
}


int SocketHandler::Select()
{
    if (m_b_check_callonconnect ||
        m_b_check_detach ||
        m_b_check_timeout ||
        m_b_check_retry ||
        m_b_check_close)
    {
        return Select(0, m_b_check_detach ? 10000 : 200000);
    }
    return Select(NULL);
}


int SocketHandler::Select(struct timeval *tsel)
{
    if (!m_add.empty())
    {
        AddIncoming();
    }
    int n = ISocketHandler_Select(tsel);
    // check CallOnConnect - EVENT
    if (m_b_check_callonconnect)
    {
        CheckCallOnConnect();
    }

#ifdef ENABLE_DETACH
    // check detach of socket if master handler - EVENT
    if (!m_slave && m_b_check_detach)
    {
        CheckDetach();
    }
#endif

    // check Connecting - connection timeout - conditional event
    if (m_b_check_timeout)
    {
        time_t tnow = time(NULL);
        if (tnow != m_tlast)
        {
            CheckTimeout(tnow);
            m_tlast = tnow;
        } // tnow != tlast
    }

    // check retry client connect - EVENT
    if (m_b_check_retry)
    {
        CheckRetry();
    }

    // check close and delete - conditional event
    if (m_b_check_close)
    {
        CheckClose();
    }

    if (!m_fds_erase.empty())
    {
        CheckErasedSockets();
    }

    // remove Add's that fizzed
    while (m_delete.size())
    {
        std::list<std::shared_ptr<Socket>>::iterator it = m_delete.begin();
        std::shared_ptr<Socket> p = *it;
        p->OnDelete();
        m_delete.erase(it);
        if (p->DeleteByHandler()
#ifdef ENABLE_DETACH
            && !(m_slave ^ p->IsDetached())
#endif
            )
        {
            p->SetErasedByHandler();
            p.reset();
        }
    }

    return n;
}


#ifdef SOCKETS_NAMESPACE
}
#endif

