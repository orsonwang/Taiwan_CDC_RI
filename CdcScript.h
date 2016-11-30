// Copyright 2015 Orson Wang(orsonwang@gmail.com). All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.
//
//  說明:
//  讀取自然人憑證的lib



#ifndef CDCScript_h
#define CDCScript_h

#include <stdio.h>

#include "common.h"

//  檢查是否為自然人憑證卡片
RtnType CDCCheck();
//  取得自然人憑證資料
RtnType CDCGetCert(char* CertContent, int* CertLength);

#endif /* CDCScript_h */
