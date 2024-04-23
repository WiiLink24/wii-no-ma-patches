#include <patch.h>

namespace room::Memory
{
ROOM_DEFINE_PATCH = {
    // Loads Room effects to MEM1 rather than BOOT memory region. The English
    // language (and many others)
    // are less concise than Japanese, causing our translations to take up more
    // memory than was allocated.
    Patch::WriteU32(0x8087bd40, 0x8087997c)};
}