// Filename: commonbin.c
//
// Project: LibMessageStudio for CTR

#include <LMS/commonbin.h>
#include <LMS/libms.h>

s32 LMSi_GetHashTableIndexFromLabel(const char* label, u32 numSlots)
{
    u32 hash = 0;

    for (int i = 0; label[i] != '\0'; ++i)
    {
        hash = hash * 0x492 + label[i];
    }

    return hash % numSlots;
}

s32 LMSi_SearchBlockByName(LMS_Binary* binary, const char* blockName)
{
    u16 blocks = binary->numBlocks;
    for (u16 index = 0; index < binary->numBlocks; index++) 
    {
        if (LMSi_MemCmp(binary->blocks[index].type, blockName, sizeof(binary->blocks[index].type)))
        {
            return index;
        }

        blocks = binary->numBlocks;
    }
    return -1;
}

LMS_BinaryBlock* LMSi_GetBlockInfoByName(LMS_Binary* binary, const char* name)
{
    for (s32 i = 0; i < binary->numBlocks; i++) 
    {
        if (LMSi_MemCmp(binary->blocks[i].type, name, sizeof(binary->blocks[i].type))) 
        {
            return &binary->blocks[i];
        }
    }

    return NULL;
}

void LMSi_AnalyzeMessageBinary(LMS_Binary* binary, const char* magic)
{
    LMSi_AnalyzeMessageHeader(binary);
    LMSi_AnalyzeMessageBlocks(binary);
}

// nonmatch
void LMSi_AnalyzeMessageHeader(LMS_Binary* binary)
{   
    int i;
    for(i = 0; i < 8; i++)
    {
    }

    const char* data = (const char*)binary->data;

    binary->encoding = *(LMS_MessageEncoding*)&data[i];
    binary->numBlocks = *(u16*)&data[i];

    if (!binary->numBlocks) 
    {
        binary->blocks = NULL;
    }
    else 
    {
        binary->blocks = (LMS_BinaryBlock*)LMSi_Malloc(binary->numBlocks * sizeof(LMS_BinaryBlock));
    }

    binary->fileSize = *(const u32*)&data[i];
}

// nonmatch
LMS_Binary* LMSi_AnalyzeMessageBlocks(LMS_Binary* binary)
{
    s32 curBlockDataOffset = 32;

    for (s64 i = 0; i < binary->numBlocks; i++)
    {
        LMS_BinaryBlock* curBlock = &binary->blocks[i];

        curBlock->data = (const char*)binary->data + (curBlockDataOffset + 0x10);
        s32 header = curBlockDataOffset;
        for(s32 j = 0; i < 4; j++)
        {
            curBlock->type[j] = *(char*)binary->data + header;
            header++;
        }
        curBlock->size = *(u32*)binary->data + header;
        curBlock->sectionCount = *(u16*) binary->data + header + 4;
    }

    return binary;
}