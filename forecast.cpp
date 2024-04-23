#include <patch.h>
#include <setting.h>

namespace room::Forecast
{
struct ForecastCode {
    u32 american_code;
    u32 japanese_code;
};

// Sourced from https://github.com/RiiConnect24/File-Maker/wiki/Weather-Icons
constexpr ForecastCode codes[] = {
    {0x0065, 0x0001}, {0x0066, 0x0002}, {0x0067, 0x0003}, {0x0068, 0x0004},
    {0x0069, 0x0004}, {0x006A, 0x000A}, {0x006B, 0x000B}, {0x006C, 0x000C},
    {0x006D, 0x000D}, {0x006E, 0x001F}, {0x006F, 0x0013}, {0x0070, 0x0014},
    {0x0071, 0x0015}, {0x0072, 0x0016}, {0x0073, 0x0016}, {0x0074, 0x001E},
    {0x0075, 0x001D}, {0x0076, 0x001E}, {0x0077, 0x001F}, {0x0078, 0x001F},
    {0x0079, 0x0013}, {0x007A, 0x000A}, {0x007B, 0x0001}, {0x007C, 0x001C},
    {0x007D, 0x0013}, {0x007E, 0x0001},
};

u32 GetLocalizedForecastCode(u32 section)
{
    LONGCALL u32 GetForecastCodeForSection(u32 section) AT(0x8010106c);
    u32 code = GetForecastCodeForSection(section);
    if (code == 0xFFFF)
        return code;

    const u32 location = sc::GetAreaCode();
    switch (location) {
    case 1:
    case 2:
        if (0x8000 < code)
            // Convert nighttime code to daytime
            code = code - 0x8000;

        for (u32 i = 0; i < sizeof(codes); i++) {
            if (codes[i].american_code == code)
                return codes[i].japanese_code;
        }

        break;
    default:
        // Japan or some other region such as China or Korea
        return code;
    }

    return code;
}

ROOM_DEFINE_PATCH = {
    Patch::WriteFunctionCall(0x8010117c, GetLocalizedForecastCode),
    Patch::WriteFunctionCall(0x80101190, GetLocalizedForecastCode),
    Patch::WriteFunctionCall(0x801011a4, GetLocalizedForecastCode)};
} // namespace room::Forecast