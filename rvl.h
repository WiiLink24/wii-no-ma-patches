#pragma once

#include <util.h>

namespace RVL {
    LONGCALL void DCFlushRange(u32* address, u32 size) AT(0x802d2150);
    LONGCALL void ICInvalidateRange(u32* address, u32 size) AT(0x802d2240);
}