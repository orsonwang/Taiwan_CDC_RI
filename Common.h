//
//  Common.h
//  smartCardUtil
//
//  Created by arc on 2015/12/2.
//  Copyright © 2015年 arc. All rights reserved.
//

#ifndef Common_h
#define Common_h

enum {
    COMMAND_SUCCESS,
    NOT_CDC_CARD,
    CARD_SUCCESS = 36864
};

enum {
    MUTE,
    OUTPUT_ERROR,
    OUTPUT_INFORMATION
};

typedef unsigned char BinByte;
typedef int RtnType;
const DER_2048_FILE_SIZE = 1192;
const DER_4096_FILE_SIZE = 2348;

#endif /* Common_h */
