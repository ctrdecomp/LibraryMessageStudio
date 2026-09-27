#pragma once

#include "LMS/commonbin.h"

typedef void* (*LMS_AllocFuncPtr)(u32 size);
typedef void (*LMS_FreeFuncPtr)(void* ptr);

void LMS_SetMemFuncs(LMS_AllocFuncPtr alloc_ptr, LMS_FreeFuncPtr free_ptr);
void* LMSi_Malloc(u32 size);
void LMSi_Free(void* ptr);
s32 LMSi_MemCmp(const void* ptr1, const void* ptr2, s32 size);
void LMSi_MemCopy(const void* dest, const void* src, s32 size);