#include <cnt.h>
#include <cstdlib.h>
#include <patch.h>
#include <setting.h>

namespace room::Localization
{
const char* GetLanguageCode()
{
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
        LONGCALL u64 GetWiiNumber(void* ptr) AT(0x80038a74);
        LONGCALL char* GetVersion() AT(0x8000dea8);
        LONGCALL char* GetRevision() AT(0x8000dea0);

        cstdlib::snprintf(
            &ctx->user_agent, 100, "WM/%s/%s/%s/%016llu", GetRevision(),
            GetVersion(), GetLanguageCode(),
            GetWiiNumber(reinterpret_cast<void*>(0x808829b8))
        );
        ctx->is_created = true;
    }

    UnlockMutex(lock, 0xffffffff);
}

ROOM_DEFINE_PATCH = {
    Patch::WriteFunctionCall(0x802af478, ReadStrapImage),
    Patch::WriteFunctionCall(0x8001d938, PatchUserAgent)};
} // namespace room::Localization
