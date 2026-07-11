#include "Strings.h"
#include <cctype>
#include <cstdio>
#include <cstring>

BitString BitStringCreate(char* str)
{
    BitString bitString = {.str = str, .size = strlen(str)};
    return bitString;
}

u64 BitStringLength(BitString* str)
{
    return str->size;
}

void BitStringChopRight(BitString* str, size_t n)
{
    if(n > str->size) n = str->size;
    str->size -= n;
}
void BitStringChopLeft(BitString* str, size_t n)
{
    if(n > str->size) n = str->size;
    str->size -= n;
    str->str += n;
}
void BitStringTrimLeft(BitString* str)
{
    while(str->size > 0 && isspace(str->str[0]))
    {
        BitStringChopLeft(str, 1);
    }
}
void BitStringTrimRight(BitString* str)
{
    while(str->size > 0 && isspace(str->str[str->size]))
    {
        BitStringChopRight(str, 1);
    }
}
void BitStringTrim(BitString* str)
{
    BitStringTrimLeft(str);
    BitStringTrimRight(str);
}
void BitStringPrint(BitString* str)
{
    printf("%.*s\n", str->size, str->str);
}
