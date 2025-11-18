#ifndef DDTOOL_H 
#define DDTOOL_H

#include "../../include/ultra64.h"

#ifndef PREFIX
    #define PREFIX
#endif

#define CAT2(a,b) a##b
#define CAT(a,b) CAT2(a,b)

#define FN(name) CAT(PREFIX, name)

void FN(ddMemcpy)(u8* src, u8* dst, int n);
void FN(ddYaz0_Decompress)(u8* src, u8* dst, int compr_size);

#define ddMemcpy FN(ddMemcpy)
#define ddYaz0_Decompress FN(ddYaz0_Decompress)

#endif // DDTOOL_H