#include <asm.h>
#include <patch.h>

namespace room::Region
{
ROOM_DEFINE_PATCH = {
    // Write mock region info as non-Japanese Wii's won't have the Japanese
    // region select channel installed.
    Patch::WriteASM(
        0x801a5dc0, 6,
        ASM_LAMBDA(
            // clang-format off
            li         r0,0x1;
            stb        r0,0x0(r3);
            li         r0,0x0;
            stb        r0,0x8(r3);
            stb        r0,0x9(r3);
            b          +140;
            // clang-format on
        )
    ),

    // Always tell the channel that the Japanese region select channel is
    // installed.
    Patch::WriteASM(
        0x802af08c, 2,
        ASM_LAMBDA(
            // clang-format off
            li         r3,0x0;
            blr;
            // clang-format on
        )
    ),

    // Removes the Wii Number region validator.
    Patch::WriteASM(0x803b4f24, 1, ASM_LAMBDA(li r3, 0x0;)),

    // Change NTSC-U PD to JPN
    Patch::WriteASM(0x8038a4f8, 1, ASM_LAMBDA(addi r30, r4, 0x434a)),
    Patch::WriteASM(0x802df030, 1, ASM_LAMBDA(addi r29, r3, 0x434a)),
    Patch::WriteASM(0x8038a788, 1, ASM_LAMBDA(addi r3, r31, 0x84)),
    Patch::WriteASM(0x8038a74c, 1, ASM_LAMBDA(addi r3, r31, 0)),

    // Change PAL PD to JPN
    Patch::WriteASM(0x8038a50c, 1, ASM_LAMBDA(addi r30, r4, 0x434a)),
    Patch::WriteASM(0x802df050, 1, ASM_LAMBDA(addi r29, r3, 0x434a)),
    Patch::WriteASM(0x8038a790, 1, ASM_LAMBDA(addi r3, r31, 0x84)),
    Patch::WriteASM(0x8038a754, 1, ASM_LAMBDA(addi r3, r31, 0))};
}