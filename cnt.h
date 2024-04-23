#pragma once

#include <patch.h>

// Leverages the compiled arc library inside the DOL.

namespace wii {
    namespace Cnt {
        struct Cnt {
            void** vtable;
            int unk;
            char _[68];
        };

        LONGCALL Cnt* ct(Cnt*) AT(0x802a35f8);
        LONGCALL Cnt* dt(Cnt*, u32) AT(0x802a3648);

        LONGCALL u32 OpenFile(Cnt* cnt, char* path, int, int) AT(0x802a3744);

        LONGCALL u32 ReadFile(Cnt* cnt, void* buffer, int size) AT(0x802a38a4);

        LONGCALL u32 CloseFile(Cnt* cnt) AT(0x802a3a74);
    }

    // I don't know what these functions do but they are in the original function
    LONGCALL void* FUN_80189b24(void*, u32) AT(0x80189b24);
    LONGCALL void* FUN_80189be0(void*, u32) AT(0x80189be0);
}

namespace ipl::nand::ReadableFile {
    LONGCALL int GetUncompressedSize(wii::Cnt::Cnt* cnt) AT(0x802a3814);
}