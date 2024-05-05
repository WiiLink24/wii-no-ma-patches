#include <asm.h>
#include <patch.h>

namespace room::Beacon
{
ROOM_DEFINE_PATCH = {
    Patch::WriteString(0x807524e0, "%sbeacon/%d"),

    // Beacon is an analytics request that is shot off during certain events,
    // or every few seconds. No data of use are sent to us; as such it is
    // discarded. The URL of the request is determined by multiple factors.
    // Being dynamic  is a problem when faced with R2 as NHTTP does not support
    // redirects. As such we will force one url. This ASM replaces the dynamic
    // part of the URL, making it always be:
    // http://prod.wiilink24.com/beacon/1
    Patch::WriteASM(0x8005d37c, 1, ASM_LAMBDA(li r7, 1))
};
}