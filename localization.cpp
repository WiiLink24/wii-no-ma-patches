#include <cnt.h>
#include <cstdlib.h>
#include <setting.h>

namespace room::Localization
{
enum Language
{
    // Languages officially supported by Nintendo
    ALL,
    PTBR,
    RU,
};

const char* GetLanguageCode()
{
    if (LANGUAGE == PTBR)
        return "ptbr";
    else if (LANGUAGE == RU)
        return "ru";

    u8 code = sc::GetLanguage();
    switch (code) {
    case 0:
        return "jp";
    case 2:
        return "Ge";
    case 3:
        return "Fr";
    case 4:
        return "Sp";
    case 5:
        return "It";
    case 6:
        return "Du";
    default:
        return "En";
    }
}

// 100% not the correct name but it does what it says at least in this context
struct StrapScreenCtx {
    void* _;
    void* tpl;
};

// Loads the strap screen image for the current language.
void ReadStrapImage(StrapScreenCtx* ctx)
{
    char unk;
    char path[32];
    wii::Cnt::Cnt cnt{};
    wii::Cnt::ct(&cnt);

    // Form path in arc
    cstdlib::snprintf(path, 32, "strapImage_%s_LZ.bin", GetLanguageCode());

    wii::Cnt::OpenFile(&cnt, path, 0xd, 1);
    wii::FUN_80189b24(&unk, 0x20);
    int size = ipl::nand::ReadableFile::GetUncompressedSize(&cnt);

    void* buffer = cstdlib::malloc(size);
    ctx->tpl = buffer;
    wii::FUN_80189be0(&unk, 0xffffffff);
    size = ipl::nand::ReadableFile::GetUncompressedSize(&cnt);

    wii::Cnt::ReadFile(&cnt, ctx->tpl, size);
    wii::Cnt::CloseFile(&cnt);

    LONGCALL void TPLBind(void* tpl) AT(0x80363a40);
    TPLBind(ctx->tpl);

    wii::Cnt::dt(&cnt, 0xffffffff);
}

// Adds the language code to the user agent.
struct Mutex {
    char _[24];
};

struct UACtx {
    Mutex mutex;
    bool is_created;
    char user_agent;
};

static_assert(sizeof(UACtx) == 26);

// Patches the User Agent for all HTTP requests to include the user's region.
void PatchUserAgent(UACtx* ctx)
{
    LONGCALL Mutex** LockMutex(Mutex * *lock, Mutex mutex) AT(0x800134d4);
    LONGCALL Mutex** UnlockMutex(Mutex * *lock, u32 val) AT(0x8001350c);

    Mutex* lock[2];
    LockMutex(lock, ctx->mutex);
    if (!ctx->is_created) {
        LONGCALL int NWC24GetMyUserId(u64* ptr) AT(0x803aba80);
        LONGCALL char* GetVersion() AT(0x8000dea8);
        LONGCALL char* GetRevision() AT(0x8000dea0);

        u64 wii_no{};
        NWC24GetMyUserId(&wii_no);

        cstdlib::snprintf(
            &ctx->user_agent, 100, "WM/%s/%s/%s/%016llu", GetRevision(),
            GetVersion(), GetLanguageCode(), wii_no);
        ctx->is_created = true;
    }

    UnlockMutex(lock, 0xffffffff);
}

u32 IsInternetChannelInstalled(char* param_1, void* param_2)
{
    LONGCALL u32 ESHasTitle(char*, void*, u32, u32) AT(0x802aedb8);

    u32 code = sc::GetAreaCode();
    switch (code) {
    case 0:
        return ESHasTitle(param_1, param_2, 0x10001, 0x4841444a);
    case 1:
        return ESHasTitle(param_1, param_2, 0x10001, 0x48414445);
    case 2:
        return ESHasTitle(param_1, param_2, 0x10001, 0x48414450);
    default:
        // Return value for not installed
        return 0;
    }
}

u32 InternetChannelTMDView(u16* param_1, void* param_2)
{
    LONGCALL u32 ESGetTMDView(u16*, void*, u32, u32) AT(0x802aeef4);

    u32 code = sc::GetAreaCode();
    switch (code) {
    case 0:
        return ESGetTMDView(param_1, param_2, 0x10001, 0x4841444a);
    case 1:
        return ESGetTMDView(param_1, param_2, 0x10001, 0x48414445);
    case 2:
        return ESGetTMDView(param_1, param_2, 0x10001, 0x48414450);
    default:
        // Return value for no view
        return 0;
    }
}

void LaunchInternetChannel(char* url)
{
    LONGCALL void OSLaunchTitleWithArguments(u32, u32, u32, char*, u32)
        AT(0x802de780);

    u32 code = sc::GetAreaCode();
    switch (code) {
    case 0:
        OSLaunchTitleWithArguments(0x10001, 0x4841444a, 0, url, 0);
    case 1:
        OSLaunchTitleWithArguments(0x10001, 0x48414445, 0, url, 0);
    default:
        // Even if this isn't PAL, this will just throw to the Wii Menu.
        // If it isn't PAL it wouldn't even reach here to begin with.
        OSLaunchTitleWithArguments(0x10001, 0x48414450, 0, url, 0);
    }

    __builtin_unreachable();
}

ROOM_DEFINE_PATCH = {
    Patch::WriteFunctionCall(0x802af478, ReadStrapImage),
    Patch::WriteFunctionCall(0x8001d938, PatchUserAgent),
    Patch::WriteFunctionCall(0x802af018, IsInternetChannelInstalled),
    Patch::WriteFunctionCall(0x802af04c, InternetChannelTMDView),
    Patch::WriteFunctionCall(0x801c0d38, LaunchInternetChannel)};
} // namespace room::Localization
