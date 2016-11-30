//
//  CdcScript.h
//  smartCardUtil
//
//  說明:
//  讀取自然人憑證的lib
//
//  Created by arc on 2015/12/2.
//  Copyright © 2015年 arc. All rights reserved.
//

#ifndef CdcScript_h
#define CdcScript_h

#include <stdio.h>

#include "Common.h"

//  檢查是否為自然人憑證卡片
RtnType CheckCdc();
//  取得自然人憑證資料
RtnType GetCdcCert(char* CertContent, int* CertLength);

#endif /* CdcScript_h */
