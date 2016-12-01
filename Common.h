// Copyright 2015 Orson Wang(orsonwang@gmail.com). All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.
//

#ifndef common_h
#define common_h

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

const int DER_2048_FILE_SIZE = 1192;
const int DER_4096_FILE_SIZE = 2348;

#endif /* common_h */
