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

#include "stdafx.h"

// General
#include "AuthCrypt.h"

// Additional
#include "HMACSHA1.h"

AuthCrypt::AuthCrypt() :
    m_ClientDecrypt(SHA_DIGEST_LENGTH), m_ServerEncrypt(SHA_DIGEST_LENGTH),
    m_Initialized(false)
{ }

void AuthCrypt::Init(BigNumber* K)
{
    uint8 ServerEncryptionKey[SEED_KEY_SIZE] = { 0xC2, 0xB3, 0x72, 0x3C, 0xC6, 0xAE, 0xD9, 0xB5, 0x34, 0x3C, 0x53, 0xEE, 0x2F, 0x43, 0x67, 0xCE };
    HmacHash serverEncryptHmac(SEED_KEY_SIZE, (uint8*)ServerEncryptionKey);
    uint8 *encryptHash = serverEncryptHmac.ComputeHash(K);

    uint8 ServerDecryptionKey[SEED_KEY_SIZE] = { 0xCC, 0x98, 0xAE, 0x04, 0xE8, 0x97, 0xEA, 0xCA, 0x12, 0xDD, 0xC0, 0x93, 0x42, 0x91, 0x53, 0x57 };
    HmacHash clientDecryptHmac(SEED_KEY_SIZE, (uint8*)ServerDecryptionKey);
    uint8 *decryptHash = clientDecryptHmac.ComputeHash(K);

    m_ClientDecrypt.Init(decryptHash);
    m_ServerEncrypt.Init(encryptHash);

    // Drop first 1024 bytes, as WoW uses ARC4-drop1024.
    uint8 syncBuf[1024];

    memset(syncBuf, 0, 1024);
    m_ServerEncrypt.UpdateData(1024, syncBuf);

    memset(syncBuf, 0, 1024);
    m_ClientDecrypt.UpdateData(1024, syncBuf);

    m_Initialized = true;
}

void AuthCrypt::DecryptRecv(uint8 *data, size_t len)
{
	if (!m_Initialized)
	{
		return;
	}

	std::lock_guard<std::mutex> lg(m_LockDecrypt);
    m_ClientDecrypt.UpdateData(len, data);
}

void AuthCrypt::EncryptSend(uint8 *data, size_t len)
{
	if (!m_Initialized)
	{
		return;
	}

	std::lock_guard<std::mutex> lg(m_LockEncrypt);
    m_ServerEncrypt.UpdateData(len, data);
}
