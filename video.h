#pragma once

#include <cstdlib.h>
#include <patch.h>


namespace room::Video
{
#pragma pack(push, 1)
struct GXRModeObj {
    u32 viTVMode;
    u16 fbWidth;
    u16 efbHeight;
    u16 xfbHeight;
    u16 viXOrigin;
    u16 viYOrigin;
    u16 viWidth;
    u16 viHeight;
    u32 xfbMode;
    u8 field_rendering;
    u8 aa;
    u8 sample_pattern[12][2];
    u8 vfilter[7];
};
#pragma pack(pop)
static_assert(sizeof(GXRModeObj) == 0x37);

// https://github.com/devkitPro/libogc/blob/fb5d0f527f5f1d92f060dce7c7018a4374e63b79/libogc/video.c#L260
// From libogc with some tweaks to make it work
GXRModeObj VideoModeFix = {
    0,
    640,
    480,
    480,
    (720 - 640)/2,
    0,
    640,
    480,
    1,
    0,
    0,
    {{6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6},
     {6, 6}},
{
    8,
    8,
    10,
    12,
    10,
    8,
    8
}
};
inline void PatchVideoMode()
{
    if (*reinterpret_cast<u32*>(0x800000cc) == 1) {
        *reinterpret_cast<u32*>(0x800000cc) = 0;
        // PAL 576i 50Hz
        cstdlib::memcpy(
            reinterpret_cast<void*>(0x806d941c), &VideoModeFix,
            sizeof(GXRModeObj)
        );
        // PAL 480i 60Hz
        cstdlib::memcpy(
            reinterpret_cast<void*>(0x806d930c), &VideoModeFix,
            sizeof(GXRModeObj)
        );
    } else if (*reinterpret_cast<u32*>(0x800000cc) == 5) {
        // PAL EuRGB60
        // Progressive mode
        *reinterpret_cast<u32*>(0x800000cc) = 0;
        VideoModeFix.xfbMode = 0;
        VideoModeFix.viTVMode = 2;

        // Fix vFilter as well
        u8 vFilter[8] =     {
            0,         // line n-1
            0,         // line n-1
           21,         // line n
           22,         // line n
           21,         // line n
            0,         // line n+1
            0          // line n+1
        };
        cstdlib::memcpy(&VideoModeFix.vfilter, vFilter, sizeof(vFilter));
        cstdlib::memcpy(
            reinterpret_cast<void*>(0x806d9394), &VideoModeFix,
            sizeof(GXRModeObj)
        );
    }
}
} // namespace room::Video