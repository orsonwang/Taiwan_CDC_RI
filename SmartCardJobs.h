// Copyright 2015 Orson Wang(orsonwang@gmail.com). All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef SmartCardJobs_h
#define SmartCardJobs_h

#include <stdio.h>

// pcsc
#include <PCSC/WinsCard.h>
#include <PCSC/pcsclite.h>

// common define
#include "common.h"

#define MAX_RECEIVE_LEN 512


RtnType GetCardResource();  
// If there's multiple readers, this function always get the card in first reader.
// For Business use, you should provide function for user to choice which reader to read.

void ReleaseCardResource();

RtnType SendApduCommandIgnoreError(BinByte *in, int len, BinByte *out, int debugMode);
// 不處理APDU指令錯誤

RtnType SendApduCommand(BinByte *in, int len, BinByte *out, BinByte *sw12, int debugMode);

RtnType ListReaders();

#endif /* SmartCardJobs_h */
