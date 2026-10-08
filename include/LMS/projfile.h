#pragma once

#include "LMS/commonbin.h"
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct LMS_ProjectBinary {
    LMS_Binary common;
    s32 clbOffset;
    s32 clrOffset;
    s32 albOffset;
    s32 atiOffset;
    s32 aliOffset;
    s32 tggOffset;
    s32 tagOffset;
    s32 tgpOffset;
    s32 tglOffset;
    s32 sylOffset;
    s32 slbOffset;
    s32 ctiOffset;
} LMS_ProjectBinary;

/* Impl Proj */

LMS_ProjectBinary* LMS_InitProject(const void* data);
void LMS_CloseProject(LMS_ProjectBinary* binary);
s32 LMS_SearchProjectBlockByName(LMS_ProjectBinary *binary, const char* blockName);

typedef struct LMS_Color
{
    u8 r;
    u8 g;
    u8 b;
    u8 a;
} LMS_Color;

typedef struct LMS_Style
{
    int regionWidth;
    int lineNum;
    int fontIndex;
    int baseColorIndex;
} LMS_Style;


typedef enum LMS_AttrType
{
    LMS_AttrType_Invalid = -1,
    LMS_AttrType_Uint8,
    LMS_AttrType_Uint16,
    LMS_AttrType_Uint32,
    LMS_AttrType_Uint8_2,
    LMS_AttrType_Uint16_2,
    LMS_AttrType_Uint32_2,
    LMS_AttrType_Float,
    LMS_AttrType_Uint16_3,
    LMS_AttrType_PrefixString_16,
    LMS_AttrType_List
} LMS_AttrType;

typedef struct LMS_AttrInfo
{
    LMS_AttrType type;
    u16 listId;
    s32 offset;
} LMS_AttrInfo;


typedef enum LMS_ColorResult
{
    LMS_ColorResult_NoColors = -5,
    LMS_ColorResult_ColorLabelNotFound = -2,
    LMS_ColorResult_IndexOutOfRange = -1,
    LMS_ColorResult_ColorFound
} LMS_ColorResult;

/* Colors */

s32 LMS_GetColorNum(LMS_ProjectBinary* binary);
LMS_ColorResult LMS_GetColor(LMS_ProjectBinary* binary, s32 id, LMS_Color* output);
s32 LMS_GetColorNum(LMS_ProjectBinary* prjBinary);
LMS_ColorResult LMS_GetColorByName(LMS_ProjectBinary* binary, const char* name, LMS_Color* output);
s32 LMS_GetColorIndexByName(LMS_ProjectBinary* binary, const char* name);

/* Attributes */

s32 LMS_GetAttrNum(LMS_ProjectBinary* prjBinary);
LMS_AttrInfo* LMS_GetAttrInfo(LMS_ProjectBinary* prjBinary, s32 id);
LMS_AttrType LMS_GetAttrType(LMS_ProjectBinary* prjBinary, s32 id);
s32 LMS_GetAttrOffset(LMS_ProjectBinary *prjBinary, s32 id);
s32 LMS_GetAttrListItemNum(LMS_ProjectBinary* prjBinary, s32 id);

/* Tag Group */

u16 LMS_GetTagGroupNum(LMS_ProjectBinary* prjBinary);
const char* LMS_GetTagGroupName(LMS_ProjectBinary* prjBinary, u16 tagGroupId);

/* Style */

s32 LMS_GetStyleNum(LMS_ProjectBinary* prjBinary);
LMS_Style* LMS_GetStyle(LMS_ProjectBinary* prjBinary, s32 id);

/* Info */

s32 LMS_GetRegionWidth(LMS_ProjectBinary* prjBinary, s32 id);
s32 LMS_GetLineNum(LMS_ProjectBinary* prjBinary, s32 id);
s32 LMS_GetFontIndex(LMS_ProjectBinary* prjBinary, s32 id);
s32 LMS_GetBaseColorIndex(LMS_ProjectBinary* prjBinary, s32 id);

/* Contents */

s32 LMS_GetContentsNum(LMS_ProjectBinary* prjBinary);
const char* LMS_GetContentPath(LMS_ProjectBinary* prjBinary, s32 id);

#ifdef __cplusplus
}
#endif
