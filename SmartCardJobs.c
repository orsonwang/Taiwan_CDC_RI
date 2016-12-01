// Copyright 2015 Orson Wang(orsonwang@gmail.com). All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include "SmartCardJobs.h"

SCARDHANDLE cardHandle;
SCARDCONTEXT cardContext;

RtnType GetCardResource(){
    uint32_t readerLen;
    char readerName[MAX_RECEIVE_LEN];
    uint32_t activeProtocol;

    // receive from card
    unsigned char receiveBuffer[MAX_RECEIVE_LEN];
    uint32_t receiveLen = MAX_RECEIVE_LEN;
    SCARD_IO_REQUEST sendProtocol, receProtocol;
    int32_t rc = 0;

    rc = SCardEstablishContext(SCARD_SCOPE_SYSTEM, 0, 0, &cardHandle);
    if (rc != SCARD_S_SUCCESS) {
        return rc;
    }

    rc = SCardListReaders(cardHandle, 0, 0, &readerLen);
    if (rc != SCARD_S_SUCCESS) {
        return rc;
    }
    rc = SCardListReaders(cardHandle, 0, readerName, &readerLen);
    if (rc != SCARD_S_SUCCESS) {
        return rc;
    }

    rc = SCardConnect( cardHandle, readerName, SCARD_SHARE_EXCLUSIVE, SCARD_PROTOCOL_T1, &cardContext, &activeProtocol);
    if (rc != SCARD_S_SUCCESS) {
        return rc;
    }

    return rc;
}

void ReleaseCardResource() {
    SCardDisconnect(cardContext, SCARD_STATE_UNPOWERED);
    SCardReleaseContext( cardHandle);
}

RtnType SendApduCommand(BinByte *in, int len, BinByte *out, BinByte *sw12, int debugMode) {
    RtnType rtn = 0;

    unsigned char receiveBuffer[MAX_RECEIVE_LEN];
    uint32_t receiveLen = sizeof(receiveBuffer);
    memset( receiveBuffer, 0x00, receiveLen);

    rtn = SCardTransmit(cardHandle, SCARD_PCI_T1, in, len, NULL, receiveBuffer, &receiveLen);
    if (rtn != SCARD_S_SUCCESS) {
        return rtn;
        ReleaseCardResource();
    }

    memcpy( out, receiveBuffer, receiveLen-2);
    sw12[0] = receiveBuffer[receiveLen-2];
    sw12[1] = receiveBuffer[receiveLen-1];

    rtn = (int)sw12[0] * 256 + (int)sw12[1];

    return rtn;
}
