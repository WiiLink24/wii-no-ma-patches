#include <algorithm>
#include <cstdlib.h>
#include <patch.h>
#include <rvl.h>
#include <util.h>
#include "video.h"

namespace room::Patch
{
extern patch PatchStart asm("_G_WiiRoomPatchStart");
extern patch PatchEnd asm("_G_WiiRoomPatchEnd");

SECTION(".apply_patches") void ApplyPatches()
{
    u32 patch_count = std::distance(&PatchStart, &PatchEnd);
    for (u32 i = 0; i < patch_count; i++) {
        ApplyPatch((&PatchStart)[i]);
    }

    // Patch PAL video mode.
    Video::PatchVideoMode();

    // Call the replaced function. I am not 100% sure what this does but if this
    // isn't called the channel freezes after the news.
    LONGCALL void SetupMEM2IThink() AT(0x802d9d40);
    SetupMEM2IThink();
}

void ApplyPatch(patch& patch)
{
    u32* address = reinterpret_cast<u32*>(patch.address);

    u32 baseArg0 =
        patch.arg0 & 0x80000000 ? patch.arg0 : 0x80001800 + patch.arg0;
    u32* arg0Ptr = reinterpret_cast<u32*>(baseArg0);
    u32 flush_size{};

    switch (patch.patch_type) {
    case WRITE:
        // Write directly to memory
        cstdlib::memcpy(address, arg0Ptr, patch.arg1);
        flush_size = patch.arg1;
        break;

    case WRITE_U32:
        *address = patch.arg0;
        flush_size = sizeof(u32);
        break;

    case FUNCTION_CALL:
        // Calculate call (bl) value
        *address = 0x48000001 | ((baseArg0 - patch.address) & 0x3FFFFFC);
        flush_size = sizeof(u32);
        break;
    }

    RVL::DCFlushRange(address, flush_size);
    RVL::ICInvalidateRange(address, flush_size);
}
} // namespace room::Patch
