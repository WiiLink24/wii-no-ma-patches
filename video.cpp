#include <asm.h>
#include <patch.h>
#include <setting.h>

namespace room::Video
{
void ForceNTSC()
{
    // We can fix the pink bar and the various PAL crashes by forcing the NTSC
    // video mode. By doing the patch like video mode, we allow for PAL users
    // to use homebrew tools to change their video mode which is nicer than what
    // we do at the moment
    if (*reinterpret_cast<u32*>(0x800000CC) == 1 ||
        *reinterpret_cast<u32*>(0x800000CC) == 5) {
        patch force_ntsc_sc_get_vi =
            Patch::WriteASM(0x802e4ba0, 2, ASM_LAMBDA(li r3, 0x0; blr;));
        Patch::ApplyPatch(force_ntsc_sc_get_vi);

        patch branch_fix = Patch::WriteU32(0x802e3f24, 0x480000c8);
        Patch::ApplyPatch(branch_fix);
    }
}

ROOM_DEFINE_PATCH = {Patch::WriteFunctionCall(0x801c0db0, ForceNTSC)};

} // namespace room::Video