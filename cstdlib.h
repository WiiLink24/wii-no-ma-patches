#pragma once

#include <util.h>

namespace cstdlib {
    LONGCALL int snprintf(char* str, u32 size, const char* format, ...) AT(0x8018fd7c);

    LONGCALL void* malloc(int size) AT(0x801c0e14);

    LONGCALL void* memcpy(void * dest, void* src, s32 n) AT(0x80004000);
}