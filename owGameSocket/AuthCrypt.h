/*
 * Copyright (C) 2008-2018 TrinityCore <https://www.trinitycore.org/>
 * Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _AUTHCRYPT_H
#define _AUTHCRYPT_H

#include "BigNumber.h"
#include "ARC4.h"

class AuthCrypt
{
    public:
        AuthCrypt();

        void Init(BigNumber* K);
        void DecryptRecv(uint8*, size_t);
        void EncryptSend(uint8*, size_t);

        bool IsInitialized() const { return m_Initialized; }

    private:
        ARC4		m_ClientDecrypt;
		std::mutex	m_LockDecrypt;
        ARC4		m_ServerEncrypt;
		std::mutex	m_LockEncrypt;

        bool		m_Initialized;
};
#endif
