// Filename: msgfile.c
//
// Project: LibMessageStudio for CTR

#include "LMS/msgfile.h"
#include "LMS/libms.h"

typedef struct TextHeader {
    u32 numMessages;
} TextHeader;

/* Inlines */

// Nonmatch
s32 LMSi_GetTextSize_UTF8(char* text)
{
    u8* cur = (u8*)text;

    while (*cur != 0)
    {
        if(!*cur) break;
        if (*cur == 14)
        {
            cur += *(u16*)(cur + 5) + 7;
        }
        else if (*cur == 15)
        {
            cur += 6;
        }
        else
        {
            cur++;
        }
    }

    return (s32)((u8*)cur - (u8*)text);
}

// Nonmatch
s32 LMSi_GetTextSize_UTF16(char* text)
{
    u16* cur = (u16*)text;

    while (*cur != 0)
    {
        if(!*cur) break;
        if (*cur == 14)
        {
            cur = (u16*)((u8*)cur + cur[3] + 8);
        }
        else if (*cur == 15)
        {
            cur = (u16*)((u8*)cur + 6);
        }
        else
        {
            cur++;
        }
    }

    return (s32)((u8*)cur - (u8*)text);
}

// Nonmatch
s32 LMSi_GetTextSize_UTF32(char* text)
{
    u32* cur = (u32*)text;

    while (*cur != 0)
    {
        if(!*cur) break;
        if (*cur == 14)
        {
            cur = (u32*)((u8*)cur + *((u16*)cur + 4) + 10);
        }
        else if (*cur == 15)
        {
            cur = (u32*)((u8*)cur + 6);
        }
        else
        {
            cur++;
        }
    }

    return (s32)((u8*)cur - (u8*)text);
}

/* LMS_Message .msbt */

LMS_MessageBinary* LMS_InitMessage(const void* data)
{
    LMS_MessageBinary* msg = (LMS_MessageBinary*)LMSi_Malloc(sizeof(LMS_MessageBinary));

    msg->common.data = data;

    LMSi_AnalyzeMessageBinary(&msg->common, "MsgStdBn");

    msg->labelsIndex = LMSi_SearchBlockByName(&msg->common, "LBL1");
    msg->textIndex = LMSi_SearchBlockByName(&msg->common, "TXT2");
    msg->attributesIndex = LMSi_SearchBlockByName(&msg->common, "ATR1");
    msg->styleIndex = LMSi_SearchBlockByName(&msg->common, "TSY1");

    return msg;
}

void LMS_CloseMessage(LMS_MessageBinary* binary)
{
    if (binary->common.blocks)
    {
        LMSi_Free(binary->common.blocks);
    }
    LMSi_Free(binary);
}

const char* LMS_GetText(LMS_MessageBinary* binary, int index)
{
    if (binary->textIndex == -1)
        return NULL;
        
    s32* txt2Data = (s32*)binary->common.blocks[binary->textIndex].data;
    if (txt2Data[0] <= index)
        return NULL;

    u32 offset = 4 + (index << 2);
    return (char*)txt2Data + *(s32*)((u32)txt2Data + offset);
}

// NON MATCHING
s32 LMS_GetTextSize(LMS_MessageBinary* binary, s32 id)
{
    char* text = (char*)LMS_GetText(binary, id);

    if (!text)
        return -1;

    switch (binary->common.encoding)
    {
    case LMS_MessageEncoding_UTF8:
        return LMSi_GetTextSize_UTF8(text);

    case LMS_MessageEncoding_UTF16:
        return LMSi_GetTextSize_UTF16(text);

    case LMS_MessageEncoding_UTF32:
        return LMSi_GetTextSize_UTF32(text);
    default:
        return -1;
    }
}

s32 LMS_GetTextNum(LMS_MessageBinary* binary)
{
    if (binary->textIndex == -1)
    {
        return -1;
    }
    return ((TextHeader*)binary->common.blocks[binary->textIndex].data)->numMessages;
}

s32 LMS_GetTextStyle(LMS_MessageBinary* binary, s32 id)
{
    if (binary->styleIndex != -1)
    {
        s32* styles = (s32*)binary->common.blocks[binary->styleIndex].data;
        return styles[id];
    }

    return -3;
}

s32 LMS_GetTextStyleByLabel(LMS_MessageBinary* binary, const char* label)
{
    s32 textIndex = LMS_GetTextIndexByLabel(binary, label);
    if(textIndex >= 0)
    {
        return LMS_GetTextStyle(binary, textIndex);
    }
    return textIndex;
}

s32 LMS_GetTextIndexByLabel(LMS_MessageBinary* msgBinary, const char* label)
{
    s32 nameLength = 0;
    if (msgBinary->labelsIndex == -1)
    {    
        return -2;
    }
    else
    {
        while (label[nameLength] != '\0') 
        {
            nameLength++;
        }

        LMS_BinaryBlock* lbl1Block = &msgBinary->common.blocks[msgBinary->labelsIndex];

        const char* lbl1Data = (const char*)lbl1Block->data;
        u32* hashTableData = (u32*)lbl1Block->data;
        s32 hashTableID = LMSi_GetHashTableIndexFromLabel(label, *lbl1Data);
        u32 labelCount = hashTableData[hashTableID * 2 + 1];
        u32 labelOffset = hashTableData[hashTableID * 2 + 2];

        if (labelCount == 0) 
        {
            return -1;
        }

        for (u32 i = 0; i <= labelCount; i++) 
        {
            s32 size = *(u8*)&lbl1Data[labelOffset];
            if (size  == nameLength && LMSi_MemCmp(label, &lbl1Data[labelOffset + 1], size) != 0) 
            {
                return *(s32*)&lbl1Data[labelOffset + size + 1];
            }
            labelOffset += size + 5;
        }
    }
    
    return -1;
}

const char* LMS_GetTextStyleLabel(LMS_MessageBinary* binary, const char* label)
{
    s32 labelIndex = LMS_GetTextIndexByLabel(binary, label);
    if(labelIndex >= 0)
    {
        return LMS_GetText(binary, labelIndex);
    }
    return NULL;
}

s32 LMS_GetAttribute(LMS_MessageBinary* binary, int index)
{
    s32 attrNum = binary->styleIndex;

    if (attrNum == -1)
    {
        return -3;
    }

    s32* data = (s32*)binary->common.blocks[attrNum].data;
    return data[index];
}

// NON-MATCHING
s32 LMS_GetLabelByTextIndex(LMS_MessageBinary* msgBinary, s32 id, const char* output)
{
    LMS_BinaryBlock* block = &msgBinary->common.blocks[ *(u32*)((char*)&msgBinary->common + 0x10)];

    const char* data = block->data;

    u32 offset = (*(u32*)data << 3) + 4;

    for (;;)
    {
        if (block->size <= offset)
        {
            return 0;
        }

        u8 length = *(u8*)&data[offset];

        u32 textIndex = *(u32*)&data[offset + 1];

        if (textIndex == id)
        {
            LMSi_MemCopy((void*)output, &data[offset + 1], length);

            ((char*)output)[length] = '\0';

            return 1;
        }

        offset += length + 5;
    }
}