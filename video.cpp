#include <util.h>
#include <patch.h>

namespace room::Video {
    void FixPinkBar() {
        LONGCALL void VIInit() AT(0x802e33e0);
        // Initialize the video then patch pink bar.
        VIInit();

        // We can fix the pink bar by forcing the 480i video mode.
        // This changes the setting for the whole system which may or may not be favourable, but it works.
        *reinterpret_cast<u8*>(0x808ad361) = 1;
    }

    ROOM_DEFINE_PATCH = {
        Patch::WriteFunctionCall(0x801c0db0, FixPinkBar)
    };
}