#pragma once

#include "LMS/types.h"

typedef struct LMS_BinaryBlock {
    const void* data;
    char type[4];
    u32 size;
    u16 sectionCount;
} LMS_BinaryBlock;

typedef enum LMS_MessageEncoding {
    LMS_MessageEncoding_UTF8,
    LMS_MessageEncoding_UTF16,
    LMS_MessageEncoding_UTF32
} LMS_MessageEncoding;

typedef struct LMS_Binary {
    const void* data;
    u32 fileSize;
    LMS_MessageEncoding encoding;
    u16 numBlocks;
    LMS_BinaryBlock* blocks;
} LMS_Binary;

typedef struct LMSBinaryInfo{
    void* unk;
} LMSBlockInfo;

s32 LMSi_GetHashTableIndexFromLabel(const char* label, u32 numSlots);
s32 LMSi_SearchBlockByName(LMS_Binary* binary, const char* blockName);
void LMSi_AnalyzeMessageBinary(LMS_Binary* binary, const char* magic);
void LMSi_AnalyzeMessageHeader(LMS_Binary* binary);
LMS_Binary* LMSi_AnalyzeMessageBlocks(LMS_Binary* binary);