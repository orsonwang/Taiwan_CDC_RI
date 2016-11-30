// Copyright 2015 Orson Wang(orsonwang@gmail.com). All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

// This sample program do a simple job. read the certification file and save it to "cdc.der" 

#include <stdio.h>
#include <stdlib.h>

#include "CDCScript.h"

int main(int argc, const char * argv[]) {
    BinByte *DERContent;
    int DERsize;

    DERContent = (BinByte *)malloc(DER_4096_FILE_SIZE * sizeof(BinByte));
    GetCdcCert(DERContent, &DERsize);

    FILE *f = fopen("./cdc.der","a+");
    fwrite(DERContent, DERsize, 1, f);
    fclose(f);   

    return 0;
}
