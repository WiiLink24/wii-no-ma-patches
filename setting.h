#pragma once

#include <patch.h>

namespace sc {
    LONGCALL u8 GetLanguage() AT(0x80307f50);
    LONGCALL u32 GetAreaCode() AT(0x80308750);
}