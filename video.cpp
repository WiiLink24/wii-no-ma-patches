#include <util.h>
#include <patch.h>
#include <rvl.h>
#include <setting.h>

namespace room::Video {
    void FixPinkBar() {
        // We can fix the pink bar by forcing the 480i video mode.
        // This changes the setting for the whole system which may or may not be favourable, but it works.
        // We must also detect if this is PAL because a video code of 0 can also be NTSC-U mode.
        if ((*reinterpret_cast<u8*>(0x808ad361) == 0 || *reinterpret_cast<u8*>(0x808ad361) == 5) && sc::GetAreaCode() == 2)
        {
            *reinterpret_cast<u8*>(0x808ad361) = 1;
            RVL::DCFlushRange(reinterpret_cast<u32*>(0x808ad361), 1);
            RVL::ICInvalidateRange(reinterpret_cast<u32*>(0x808ad361), 1);
        }

        LONGCALL void VIInit() AT(0x802e33e0);

        // Now initialize the video
        VIInit();
    }

    ROOM_DEFINE_PATCH = {
        Patch::WriteFunctionCall(0x801c0db0, FixPinkBar)
    };
}