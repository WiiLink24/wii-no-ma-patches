#include <patch.h>

namespace room::DSi
{
ROOM_DEFINE_PATCH = {
    Patch::WriteASM(0x80014f88, 1, ASM_LAMBDA(lis r3, 0xfff;))
};
}