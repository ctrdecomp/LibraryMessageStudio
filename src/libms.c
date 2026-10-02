// Filename: libms.c
//
// Project: LibMessageStudio for CTR

#include <LMS/libms.h>

LMS_AllocFuncPtr LMSi_sAllocFuncPtr;
LMS_FreeFuncPtr LMSi_sFreeFuncPtr;

void* LMSi_Malloc(u32 size)
{
    return LMSi_sAllocFuncPtr(size);
}

void LMSi_Free(void* ptr)
{
    LMSi_sFreeFuncPtr(ptr);
}

void LMS_SetMemFuncs(LMS_AllocFuncPtr alloc_ptr, LMS_FreeFuncPtr free_ptr)
{
    LMSi_sAllocFuncPtr = alloc_ptr;
    LMSi_sFreeFuncPtr = free_ptr;
}

void LMSi_MemCopy(const void* dest, const void* src, s32 size)
{
    for (int i = 0; i < size; i++)
    {
        ((u8*)dest)[i] = ((u8*)src)[i];
    }
}

s32 LMSi_MemCmp(const void* ptr1, const void* ptr2, s32 size)
{
    const s8* ptr1Char = (const s8*)ptr1;
    const s8* ptr2Char = (const s8*)ptr2;
    for(int i = 0; i < size; i++)
    {
        if(ptr1Char[i] != ptr2Char[i])
        {
            return 0;
        }
    }
    return 1;
}