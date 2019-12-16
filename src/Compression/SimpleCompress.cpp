#include "stdafx.h"
#include "SimpleCompress.h"
#include "DefineFiles\DefineFile.h"

#include "Utility\Utility.h"

using namespace EricCore;

SimpleCompress::SimpleCompress() {
}


SimpleCompress::~SimpleCompress() {
}

int SimpleCompress::find_duplicate_value(eu32_p src, eu32 maxLen) {

    eu32* srcPtr = (eu32*)src;


    int patternNo = 0; // no pattern
    int matchCnt = 0;

    eu32 value = NULL_32;
    eu32 patternLen = NULL_32;

    // compress the same pattern
    for (eu32 i = 1; i < maxLen; i++) {
        eu32 previousValue = VALUE_OF_PTR(eu32, srcPtr + i - 1);
        eu32 currValue = VALUE_OF_PTR(eu32, srcPtr + i);

        if (previousValue == currValue) {
            matchCnt++;
        } else {
            return i;
        }
    }
    return 0;
}

int SimpleCompress::compress(eu32_p src, eu32_p desc, eu32 maxLen) {
    eu32* srcPtr = (eu32*)src;
    eu32* descPtr = (eu32*)desc;
    eu32 totalLen = maxLen;

    int orgDescPtr = (int)descPtr;

    int noPaternCnt = 0;
    int noPatternStartIdx = -1;

    // compress the same pattern
    for (eu32 i = 0; i < totalLen; i++) {
        int matchCnt = find_duplicate_value(srcPtr + i, totalLen);

        if (matchCnt >= 0x100) {

            if (noPaternCnt > 0) {

                descPtr[0] = 2; // no pattern
                descPtr[1] = noPatternStartIdx;
                descPtr[2] = noPaternCnt;
                descPtr[3] = NULL_32;
                descPtr += 4;

                for (int j = 0; j < noPaternCnt; j++) {
                    descPtr[j] = srcPtr[noPatternStartIdx +j];
                }
                descPtr += noPaternCnt;
                noPatternStartIdx = -1;
            }
    
            descPtr[0] = 1; // the same pattern
            descPtr[1] = i; 
            descPtr[2] = matchCnt;
            descPtr[3] = srcPtr[i];
            descPtr += 0x4;

            i += matchCnt - 1;
        } else {
            //hit noPattern
            if (noPatternStartIdx < 0) {
                //init noPattern param
                noPatternStartIdx = i;
                noPaternCnt = 0;
            }
            noPaternCnt++;
        }
    }
    return ((int)descPtr - orgDescPtr)/4;
}

int SimpleCompress::decompress(eu32_p src, eu32_p desc, eu32 maxLen) {

    //eu32 srcBuf[4096];
    //eu32 descBuf[4096];
    //memcpy(srcBuf, src, 4096);

    eu32_p srcBuf = src;
    eu32_p descBuf = desc;

    eu32 srcIdx = 0;
    eu32 descIdx = 0;

    while (srcIdx < maxLen) {
        eu32 patternNo = srcBuf[srcIdx + 0];
        eu32 startIdx = srcBuf[srcIdx + 1];
        eu32 matchCnt = srcBuf[srcIdx + 2];
        eu32 value = srcBuf[srcIdx + 3];
        srcIdx += 4;

        if (patternNo == 1) {
            for (eu32 j = 0; j < matchCnt; j++) {
                descBuf[descIdx + j] = value;
            }
            descIdx += matchCnt;
        } else if (patternNo == 2) {
            for (eu32 j = 0; j < matchCnt; j++) {
                descBuf[descIdx + j] = srcBuf[srcIdx + j];
            }
            descIdx += matchCnt;
            srcIdx += matchCnt;
        } else {
            throw exception("decompress error");
        }
    }
    return 0;
}