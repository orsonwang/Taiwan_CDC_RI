//
//  SmartCardJobs.h
//  smartCardUtil
//
//  Created by arc on 2015/12/2.
//  Copyright © 2015年 arc. All rights reserved.
//

#ifndef SmartCardJobs_h
#define SmartCardJobs_h

#include <stdio.h>

// pcsc
#include <PCSC/WinsCard.h>
#include <PCSC/pcsclite.h>

// common define
#include "Common.h"

#define MAX_RECEIVE_LEN 512


RtnType GetCardResource();  // always get the card of the first reader
void ReleaseCardResource();

// 不處理APDU指令錯誤
RtnType SendApduCommandIgnoreError(BinByte *in, int len, BinByte *out, int debugMode);
RtnType SendApduCommand(BinByte *in, int len, BinByte *out, BinByte *sw12, int debugMode);

RtnType ListReaders();

#endif /* SmartCardJobs_h */
