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
BinByte InitStep[7] = {0x80, 0xA4, 0x00, 0x00, 0x02, 0x3F, 0x00}; //select MF
BinByte Start1[7] = {0x80, 0xA4, 0x00, 0x00, 0x02, 0x09, 0x00};
BinByte Start2[7] = {0x80, 0xA4, 0x00, 0x00, 0x02, 0x09, 0x05};
BinByte GetDataString[5] = {0x80, 0xB0, 0x00, 0x00, 0x14};
// summary of check cdc card
BinByte* CheckSteps[] = { InitStep, Start1, Start2, GetDataString };
int CheckStepLens[__CHCEK_STEPS_] = { 7,7,7,5};

BinByte SelectCertEf1[7] = { 0x80, 0xA4, 0x00, 0x00, 0x02, 0x50, 0x30}; //select DF (PKCS#15)
BinByte SelectCertEf2[7] = { 0x80, 0xA4, 0x00, 0x00, 0x02, 0x08, 0xF2}; //select CERT store EF
BinByte GetRootCALength[5] = { 0x80, 0xB0, 0x00, 0x00, 0x04}; //Root CA
BinByte GetCALength[5] = { 0x80, 0xB0, 0x07, 0x00, 0x04};  // Issuer CA
BinByte GetCert1Length[5] = { 0x80, 0xB0, 0x0E, 0x00, 0x04}; // User CA for Encrypt? (waiting for confirm)
BinByte GetCert2Length[5] = { 0x80, 0xB0, 0x15, 0x00, 0x04}; // User CA for Signature
// summary of get cert
BinByte* GetRootCASteps[] = { InitStep, SelectCertEf1, SelectCertEf2, GetRootCALength};
BinByte* GetCASteps[] = { InitStep, SelectCertEf1, SelectCertEf2, GetCALength};
BinByte* GetCert1Steps[] = { InitStep, SelectCertEf1, SelectCertEf2, GetCert1Length};
BinByte* GetCert2Steps[] = { InitStep, SelectCertEf1, SelectCertEf2, GetCert2Length};
int GetCertStepLens[__GETCERT_STEPS_] = { 7, 7, 7, 5};


RtnType CDCCheck(){
    RtnType rtn = COMMAND_SUCCESS;
    BinByte receiveBuffer[MAX_RECEIVE_LEN];
    BinByte sw12[2];

//    TODO: Error Handle
    GetCardResource();

    for( int i = 0; i < __CHCEK_STEPS_; ++i){
        memset( receiveBuffer, 0x00, MAX_RECEIVE_LEN);
        memset( sw12, 0x00, 2);
        rtn = SendApduCommand(CheckSteps[i], CheckStepLens[i], receiveBuffer, sw12);
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

int CDCGetCertProcedure(BinByte* certStore, int caStart, int offset, BinByte len){
    int rtn = 0;
    int start = caStart + offset;
    BinByte* index;
    index = &start;
    char sw12[4];

    //    printf( "tail:%02X, head:%02X\n", index[0], index[1]);

    BinByte command[] = { 0x80, 0xB0, index[1], index[0], len};
    BinByte receiveBuffer[256];
    uint32_t receiveLen = 256;
    int32_t rc = 0;
    memset( receiveBuffer, 0x00, receiveLen);

    rc = SendApduCommand(command, 5, receiveBuffer, sw12);

    memcpy( certStore, receiveBuffer, len);

    return rtn;
}


RtnType CDCGetCert2(char* CertContent, int* CertLength){
    RtnType rtn = COMMAND_SUCCESS;
    BinByte receiveBuffer[MAX_RECEIVE_LEN];
    BinByte sw12[4];

    GetCardResource();

//    取得憑證長度
    for( int i = 0; i < __GETCERT_STEPS_; ++i){
        memset( receiveBuffer, 0x00, MAX_RECEIVE_LEN);
        memset( sw12, 0x00, 4);
        rtn = SendApduCommand(GetCert2Steps[i], GetCertStepLens[i], receiveBuffer, sw12);
	//取得檔案長度，正確的回傳值會是30 82 xx xx，後面這兩位就是檔案長度
        if( rtn != COMMAND_SUCCESS && rtn != CARD_SUCCESS)
            break;
    }

//    依區塊大小，逐次將憑證取出，取出的憑證格式為DER
    if( rtn == COMMAND_SUCCESS || rtn == CARD_SUCCESS) {
        int certLen = receiveBuffer[2]*256+receiveBuffer[3] + 4; // 4是為了補回30 82 xx xx 這四個byte
        int chunkLen = __CHUNK_SIZE;
        int index = 0;
        BinByte certTmp[certLen];

        while(index < certLen){
            int toEndLen = certLen-index;
            chunkLen = (chunkLen<(toEndLen))?chunkLen:toEndLen;
            CDCGetCertProcedure(certTmp+index, 0x1500, index, chunkLen); // 請注意0x1500這個魔術數字，他應該是用getCert2Length[2]跟getCert2Length[3]來組成
	    								 // 如果要讀取其他憑證，就需要做相對應的調整
            index += chunkLen;
        }

        memcpy(CertContent, certTmp, certLen);
        *CertLength = certLen;
    }

    ReleaseCardResource();
    return rtn;
}
