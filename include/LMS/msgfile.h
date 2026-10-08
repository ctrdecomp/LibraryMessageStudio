#pragma once

#include "LMS/commonbin.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct LMS_MessageBinary {
    LMS_Binary common;
    s32 labelsIndex;
    s32 textIndex;
    s32 attributesIndex;
    s32 styleIndex;
} LMS_MessageBinary;

LMS_MessageBinary* LMS_InitMessage(const void* data);
void LMS_CloseMessage(LMS_MessageBinary* binary);

const char* LMS_GetText(LMS_MessageBinary* binary, int index);
s32 LMS_GetTextSize(LMS_MessageBinary* binary, s32 id);
s32 LMS_GetTextNum(LMS_MessageBinary* binary);
s32 LMS_GetTextStyle(LMS_MessageBinary* binary, s32 id);
s32 LMS_GetTextStyleByLabel(LMS_MessageBinary* binary, const char* label);
s32 LMS_GetTextIndexByLabel(LMS_MessageBinary* binary, const char* label);
const char* LMS_GetTextByLabel(LMS_MessageBinary* binary, const char* label);

s32 LMS_GetAttribute(LMS_MessageBinary* binary, int index);
s32 LMS_GetLabelByTextIndex(LMS_MessageBinary* msgBinary, s32 id, const char* output);

#ifdef __cplusplus
}
#endif
