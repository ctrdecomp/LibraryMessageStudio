// Filename: projfile.c
//
// Project: LibMessageStudio for CTR
//
// TODO: Implement all inlines for this.

#include <LMS/projfile.h>
#include <LMS/libms.h>

// Impl Proj //

LMS_ProjectBinary* LMS_InitProject(const void* data)
{
    LMS_ProjectBinary* binary = (LMS_ProjectBinary*)LMSi_Malloc(sizeof(LMS_ProjectBinary));

    binary->common.data = data;

    LMSi_AnalyzeMessageBinary(&binary->common, "MsgPrjBn", 4);

    binary->clrOffset = LMSi_SearchBlockByName(&binary->common, "CLR1");
    binary->clbOffset = LMSi_SearchBlockByName(&binary->common, "CLB1");
    binary->atiOffset = LMSi_SearchBlockByName(&binary->common, "ATI2");
    binary->albOffset = LMSi_SearchBlockByName(&binary->common, "ALB1");
    binary->aliOffset = LMSi_SearchBlockByName(&binary->common, "ALI2");
    binary->tggOffset = LMSi_SearchBlockByName(&binary->common, "TGG2");
    binary->tagOffset = LMSi_SearchBlockByName(&binary->common, "TAG2");
    binary->tgpOffset = LMSi_SearchBlockByName(&binary->common, "TGP2");
    binary->tglOffset = LMSi_SearchBlockByName(&binary->common, "TGL2");
    binary->sylOffset = LMSi_SearchBlockByName(&binary->common, "SYL3");
    binary->slbOffset = LMSi_SearchBlockByName(&binary->common, "SLB1");
    binary->ctiOffset = LMSi_SearchBlockByName(&binary->common, "CTI1");

    return binary;
}

void LMS_CloseProject(LMS_ProjectBinary* binary)
{
    if (binary->common.blocks) 
    {
        LMSi_Free(binary->common.blocks);
    }
    
    LMSi_Free(binary);
}

s32 LMS_SearchProjectBlockByName(LMS_ProjectBinary* binary, const char* blockName)
{
    return LMSi_SearchBlockByName(&binary->common, blockName);
}

// Colors //

s32 LMS_GetColorNum(LMS_ProjectBinary* prjBinary)
{
    if (prjBinary->clrOffset != -1) 
    {
        return *(s32*)(prjBinary->common).blocks[prjBinary->clrOffset].data;
    }

    return 0;
}

LMS_ColorResult LMS_GetColor(LMS_ProjectBinary* prjBinary, s32 id, LMS_Color* output)
{
    if (prjBinary->clrOffset == -1) 
    {
        return LMS_ColorResult_NoColors;
    }

    const char* colorData = (const char*)prjBinary->common.blocks[prjBinary->clrOffset].data;

    s32 colorCount = *(s32*)colorData;

    if (id >= colorCount) 
    {
        return LMS_ColorResult_IndexOutOfRange;
    }

    colorData += (s64)id * 4;

    output->r = colorData[4];
    output->g = colorData[5];
    output->b = colorData[6];
    output->a = colorData[7];

    return LMS_ColorResult_ColorFound;
}

// Attributes //

s32 LMS_GetAttrNum(LMS_ProjectBinary* prjBinary)
{
    if (prjBinary->atiOffset != -1) 
    {
        return *(s32*)(prjBinary->common).blocks[prjBinary->clrOffset].data;
    }

    return 0;
}

LMS_AttrInfo* LMS_GetAttrInfo(LMS_ProjectBinary* prjBinary, s32 id)
{
    if (prjBinary->atiOffset != -1) 
    {
        const char* attrData = (const char*)prjBinary->common.blocks[prjBinary->atiOffset].data;
        LMS_AttrInfo* attrInfos = (LMS_AttrInfo*)&attrData[4];

        if (id < *(s32*)attrData) 
        {
            LMS_AttrInfo* attrInfo = &attrInfos[id];

            if (!attrInfo) 
            {
                return NULL;
            }
    
            return attrInfo;
        }
    }
    
    return NULL;
}

LMS_AttrType LMS_GetAttrType(LMS_ProjectBinary* prjBinary, s32 id)
{
    LMS_AttrInfo* attrInfo = LMS_GetAttrInfo(prjBinary, id);

    if (!attrInfo) 
    {
        return (u8)-1;
    }

    return attrInfo->type;
}

s32 LMS_GetAttrOffset(LMS_ProjectBinary *prjBinary, s32 id)
{
    LMS_AttrInfo* attrInfo = LMS_GetAttrInfo(prjBinary, id);

    if (!attrInfo) 
    {
        return -1;
    }

    return attrInfo->offset;
}

s32 LMS_GetAttrListItemNum(LMS_ProjectBinary* prjBinary, s32 id)
{
    LMS_AttrInfo* attrInfo = LMS_GetAttrInfo(prjBinary, id);

    if (!attrInfo) 
    {
        return 0;
    }

    if (attrInfo->type == 9) 
    {
        if (prjBinary->aliOffset == -1) 
        {
            return 0;
        }

        const char* ali2Data = (const char*)prjBinary->common.blocks[prjBinary->aliOffset].data;

        s32 listItemNum = *(s32*)&ali2Data[*(u32*)&ali2Data[attrInfo->listId * 4 + 4]];

        return listItemNum;
    }

    return 0;
}

// Tags //

u16 LMS_GetTagGroupNum(LMS_ProjectBinary* prjBinary)
{
    if (prjBinary->tggOffset != -1) 
    {
        return *(u16*)prjBinary->common.blocks[prjBinary->tggOffset].data;
    }

    return 0;
}

const char* LMS_GetTagGroupName(LMS_ProjectBinary* prjBinary, u16 tagGroupId)
{
    //! TODO
}

// Styles //

s32 LMS_GetStyleNum(LMS_ProjectBinary* prjBinary)
{
    if (prjBinary->sylOffset != -1) 
    {
        return *(s32*)(prjBinary->common).blocks[prjBinary->sylOffset].data;
    }

    return 0;
}

LMS_Style* LMS_GetStyle(LMS_ProjectBinary* prjBinary, s32 id)
{
    if (prjBinary->sylOffset != -1) 
    {
        const char* styleData = (const char*)prjBinary->common.blocks[prjBinary->sylOffset].data;
        LMS_Style* styles = (LMS_Style*)&styleData[4];

        if (id < *(u32*)styleData) 
        {
            return &styles[id];
        }
    }

    return NULL;
}

// Info //

s32 LMS_GetRegionWidth(LMS_ProjectBinary* prjBinary, s32 id)
{
    if (prjBinary->sylOffset == -1) 
    {
        return -1;
    }
    
    const char* styleData = (const char*)prjBinary->common.blocks[prjBinary->sylOffset].data;
    LMS_Style* styles = (LMS_Style*)&styleData[4];
    if (id < *(u32*)styleData) 
    {
        return styles[id].regionWidth;
    }
    
    return -1;
}

s32 LMS_GetLineNum(LMS_ProjectBinary* prjBinary, s32 id)
{
    if (prjBinary->sylOffset == -1) 
    {
        return -1;
    }
    
    const char* styleData = (const char*)prjBinary->common.blocks[prjBinary->sylOffset].data;
    LMS_Style* styles = (LMS_Style*)&styleData[4];
    if (id < *(u32*)styleData) 
    {
        return styles[id].lineNum;
    }
    
    return -1;
}

s32 LMS_GetFontIndex(LMS_ProjectBinary* prjBinary, s32 id)
{
    if (prjBinary->sylOffset == -1) 
    {
        return -1;
    }
    
    const char* styleData = (const char*)prjBinary->common.blocks[prjBinary->sylOffset].data;
    LMS_Style* styles = (LMS_Style*)&styleData[4];
    if (id < *(u32*)styleData) 
    {
        s32 fontIndex = -12;

        if (styles[id].fontIndex != -1) 
        {
            fontIndex = styles[id].fontIndex;
        }

        return fontIndex;
    }
    
    return -1;
}

s32 LMS_GetBaseColorIndex(LMS_ProjectBinary* prjBinary, s32 id)
{
    if (prjBinary->sylOffset == -1) 
    {
        return -1;
    }
    
    const char* styleData = (const char*)prjBinary->common.blocks[prjBinary->sylOffset].data;
    LMS_Style* styles = (LMS_Style*)&styleData[4];
    if (id < *(u32*)styleData) 
    {
        if (styles[id].baseColorIndex != -1) 
        {
            return styles[id].baseColorIndex;
        }

        return -12;
    }
    
    return -1;
}

// Contents // 

s32 LMS_GetContentsNum(LMS_ProjectBinary* prjBinary)
{
    if (prjBinary->ctiOffset != -1) 
    {
        return *(s32*)(prjBinary->common).blocks[prjBinary->ctiOffset].data;
    }

    return -1;
}

const char* LMS_GetContentPath(LMS_ProjectBinary* prjBinary, s32 id)
{
    if (prjBinary->ctiOffset == -1) 
    {
        return NULL;
    }

    const char* data = (const char*)prjBinary->common.blocks[prjBinary->ctiOffset].data;

    if (id < *(s32*)data) 
    {
        u32* contentOffsets = (u32*)&data[4];

        return &data[contentOffsets[id * 1]];
    }
    
    return NULL;
}