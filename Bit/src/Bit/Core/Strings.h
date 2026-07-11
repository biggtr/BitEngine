#pragma once
#include "Bit/Core/Defines.h"
#include <cstddef>

struct BitString
{
    char* str;
    u64 size;
};


BitString BitStringCreate(char* str);
BitString BitStringDestroy();

u64 BitStringLength();

void BitStringChopRight(BitString* str, size_t n);
void BitStringChopLeft(BitString* str, size_t n);
void BitStringTrimLeft(BitString* str);
void BitStringTrimRight(BitString* str);
void BitStringPrint(BitString* str);



