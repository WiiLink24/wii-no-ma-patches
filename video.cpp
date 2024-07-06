#include <asm.h>
#include <patch.h>

namespace room::Video
{
ROOM_DEFINE_PATCH = {
    Patch::WriteASM(0x802e4ba0, 2, ASM_LAMBDA(li r3, 0x0; blr;)),
    Patch::WriteU32(0x802e3f24, 0x480000c8)};
}