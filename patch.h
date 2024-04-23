#pragma once

#include <util.h>

extern "C" {
enum PatchType : u32 {
    FUNCTION_CALL,
    WRITE,
    WRITE_U32
};


typedef struct  {
    PatchType patch_type;
    u32 address;

    union {
        u32 arg0;
        const void* arg0p;
        const u32* arg0p32;
    };

    u32 arg1;
} PACKED patch;
}

namespace room::Patch {

void ApplyPatch(patch& patch);

constexpr patch WriteFunctionCall(u32 address, auto function) {
  return patch{
    .patch_type = FUNCTION_CALL,
    .address = address,
    .arg0 = u32(+function)
  };
}

template<u32 N>
constexpr patch WriteString(u32 address, const char (&string)[N]) {
  return patch{
    .patch_type = WRITE,
    .address = address,
    .arg0p = string,
    .arg1 = sizeof(string)
  };
}

constexpr patch WriteASM(u32 address, u32 instruction_count, auto function)
{
  return patch{
    .patch_type = WRITE,
    .address = address,
    .arg0 = u32(+function),
    .arg1 = instruction_count * sizeof(u32),
  };
}

constexpr patch WriteU32(u32 address, u32 value)
{
  return patch{
    .patch_type = WRITE_U32,
    .address = address,
    .arg0 = value,
    .arg1 = sizeof(u32)
  };
}

#define _ROOM_DEFINE_PATCH2(NUM)                                               \
    __attribute__((__section__(".room_patch"))) patch __room_patch_##NUM[]

#define _ROOM_DEFINE_PATCH1(NUM) _ROOM_DEFINE_PATCH2(NUM)
#define ROOM_DEFINE_PATCH _ROOM_DEFINE_PATCH1(__COUNTER__)
}