#pragma once

#include <util.h>

namespace room::Encryption {
using func = void (*)();

static constexpr u8 PERSONAL_DATA_KEY[16] = {
    // Set 1
    0x88, 0x7C, 0x13, 0x5A, 0x85, 0x9D, 0x41, 0x00,
    // Set 2
    0xA1, 0x82, 0x32, 0x89, 0x4A, 0xC8, 0x25, 0x00};

// Similarly so, replace the last 4 bytes here with the device's ID.
static constexpr u8 PERSONAL_DATA_IV[16] = {
    0xb2, 0xb4, 0xc4, 0x5d, 0x27, 0xcd, 0x45, 0x9d,
    0x86, 0x1a, 0xba, 0x25, 0x00, 0x00, 0x00, 0x00,
};

#pragma pack(push, 1)
struct StringBuffer {
    u8 _[8];
    char* string;
};

struct AESCtx {
    u8 _[13];
    void* dst;
    u8 unk[239];
    func* function;
    int something;
};
#pragma pack(pop)

LONGCALL void AESCrypt(AESCtx* ctx, void* out, void* in, u32 length) AT(0x803b7de4);
LONGCALL int NETAESCreateEx(AESCtx* ctx, const u8* key, u32 key_size, const u8* iv) AT(0x803b7dd8);

LONGCALL void* memcpy(void* out, const u8* in, u32 size) AT(0x80004000);
LONGCALL void* memset(void *__s, u32 __c, u32 __n) AT(0x80004350);
LONGCALL u8* malloc(u32 size) AT(0x801c0e14);
}