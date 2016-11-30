// Copyright 2015 Orson Wang(orsonwang@gmail.com). All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.


#include "CDCScript.h"
#include "SmartCardJobs.h"

#define __CHCEK_STEPS_ 4
#define __GETCERT_STEPS_ 4
#define __CHUNK_SIZE 200

int initOffset = 5376;

// command set
BinByte InitStep[7] = {0x80, 0xA4, 0x00, 0x00, 0x02, 0x3F, 0x00};
BinByte Start1[7] = {0x80, 0xA4, 0x00, 0x00, 0x02, 0x09, 0x00};
BinByte Start2[7] = {0x80, 0xA4, 0x00, 0x00, 0x02, 0x09, 0x05};
BinByte GetDataString[5] = {0x80, 0xB0, 0x00, 0x00, 0x14};
// summary of check cdc card
BinByte* CheckSteps[] = { InitStep, Start1, Start2, GetDataString };
int CheckStepLens[__CHCEK_STEPS_] = { 7,7,7,5};

BinByte SelectCertEf1[7] = { 0x80, 0xA4, 0x00, 0x00, 0x02, 0x50, 0x30};
BinByte SelectCertEf2[7] = { 0x80, 0xA4, 0x00, 0x00, 0x02, 0x08, 0xF2};
BinByte GetCertLength[5] = { 0x80, 0xB0, 0x15, 0x02, 0x02};
BinByte GetCdcCertTemplate[5] = { 0x80, 0xB0, 0x00, 0x00, 0x00};
// summary of get cert
BinByte* GetCertSteps[] = { InitStep, SelectCertEf1, SelectCertEf2, GetCertLength};
int GetCertStepLens[__GETCERT_STEPS_] = { 7, 7, 7, 5};


RtnType CDCCheck(){
    RtnType rtn = AITC_SUCCESS;
    BinByte receiveBuffer[MAX_RECEIVE_LEN];
    BinByte sw12[2];
    
//    TODO: Error Handle
    GetCardResource();
    
    for( int i = 0; i < __CHCEK_STEPS_; ++i){
        memset( receiveBuffer, 0x00, MAX_RECEIVE_LEN);
        memset( sw12, 0x00, 2);
        rtn = SendApduCommand(CheckSteps[i], CheckStepLens[i], receiveBuffer, sw12, MUTE);
        if( rtn != COMMAND_SUCCESS && rtn != CARD_SUCCESS)
            break;
    }
//    若字串不為cht V32N則不為自然人憑證卡片
    if( rtn == COMMAND_SUCCESS || rtn == CARD_SUCCESS) {
        BinByte CDCString[] = { 0x43, 0x48, 0x54, 0x20, 0x56, 0x33, 0x32, 0x4E };
        if( memcmp( receiveBuffer,  CDCString, 8) != 0){
            rtn = NOT_CDC_CARD;
        }
    }
    
    ReleaseCardResource();
    return rtn;
}

int CDCGetCertProcedure(BinByte* certStore, int offset, BinByte len){
    int rtn = 0;
    int start = 5376 + offset;
    BinByte* index;
    index = &start;
    char sw12[4];
    
    //    printf( "tail:%02X, head:%02X\n", index[0], index[1]);
    
    BinByte command[] = { 0x80, 0xB0, index[1], index[0], len};
    BinByte receiveBuffer[256];
    uint32_t receiveLen = 256;
    int32_t rc = 0;
    memset( receiveBuffer, 0x00, receiveLen);
    
    rc = SendApduCommand(command, 5, receiveBuffer, sw12, MUTE);
    
    memcpy( certStore, receiveBuffer, len);
    
    return rtn;
}


RtnType CDCGetCert(char* CertContent, int* CertLength){
    RtnType rtn = AITC_SUCCESS;
    BinByte receiveBuffer[MAX_RECEIVE_LEN];
    BinByte sw12[2];
    
    GetCardResource();
    
//    取得憑證長度
    for( int i = 0; i < __GETCERT_STEPS_; ++i){
        memset( receiveBuffer, 0x00, MAX_RECEIVE_LEN);
        memset( sw12, 0x00, 2);
        rtn = SendApduCommand(GetCertSteps[i], GetCertStepLens[i], receiveBuffer, sw12, MUTE);
        if( rtn != COMMAND_SUCCESS && rtn != CARD_SUCCESS)
            break;
    }

//    依區塊大小，逐次將憑證取出，取出的憑證格式為DER
    if( rtn == COMMAND_SUCCESS || rtn == CARD_SUCCESS) {
        int certLen = receiveBuffer[0]*256+receiveBuffer[1] + 4;
        int chunkLen = __CHUNK_SIZE;
        int index = 0;
        BinByte certTmp[certLen];
        
        while(index < certLen){
            int toEndLen = certLen-index;
            chunkLen = (chunkLen<(toEndLen))?chunkLen:toEndLen;
            CDCGetCertProcedure(certTmp+index, index, chunkLen);
            index += chunkLen;
        }

    }
    memcpy(CertContent, certTmp, certLen);
    *CertLength = certLen;

    ReleaseCardResource();
    return rtn;
}
