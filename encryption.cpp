#include <encryption.h>
#include <patch.h>

namespace room::Encryption
{

u8* PKCSPadding(const u8* input, u32 input_len, u32* padded_len)
{
    LONGCALL void OSFatal(void*, void*, const char* message) AT(0x802d52a0);

    u32 pad_len = 16 - (input_len % 16);
    *padded_len = input_len + pad_len;

    u8* padded_input = malloc(*padded_len);
    if (!padded_input) {
        OSFatal(
            reinterpret_cast<void*>(0x80885e84),
            reinterpret_cast<void*>(0x80885e80),
            "Memory allocation error. Please restart your Wii by holding the "
            "POWER button."
        );
        __builtin_unreachable();
    }

    memcpy(padded_input, input, input_len);
    memset(padded_input + input_len, pad_len, pad_len);

    return padded_input;
}

char* EncryptAndCopyStringToBuffer(StringBuffer* buffer, char* string)
{
    // Encrypt the string first
    AESCtx ctx{};
    NETAESCreateEx(
        &ctx, PERSONAL_DATA_KEY, sizeof(PERSONAL_DATA_KEY), PERSONAL_DATA_IV
    );

    // There is no overarching encryption function in the DOL, by setting the
    // function pointer to this address, AESCrypt will act as encryption rather
    // than decryption.
    *reinterpret_cast<u32*>(0x80886354) = 0x803b7e9c;

    LONGCALL u32 strlen(char* str) AT(0x802b46e8);
    u32 padded_len{};
    u8* padded_string = PKCSPadding(
        reinterpret_cast<const u8*>(string), strlen(string), &padded_len
    );

    u8* encrypted = malloc(padded_len);

    AESCrypt(&ctx, encrypted, padded_string, padded_len);

    // Convert bytes to string
    u32 j = 0;
    constexpr char hex_table[] = "0123456789abcdef";
    u8* encoded_string = malloc(padded_len * 2 + 1);
    for (u32 i = 0; i < padded_len; i++) {
        encoded_string[j] = hex_table[encrypted[i] >> 4];
        encoded_string[j + 1] = hex_table[encrypted[i] & 0x0f];
        j += 2;
    }

    // Ensure null terminator
    encoded_string[padded_len * 2] = '\0';

    char* dest = buffer->string;
    LONGCALL char* strcpy(char* __dest, char* __src) AT(0x802be404);
    strcpy(dest, reinterpret_cast<char*>(encoded_string));

    // For whatever reason, Nintendo decided to use this method for the string
    // buffer. This is an incredibly bad idea as in theory it could try
    // and copy a string that is larger than available memory and crash
    // everything. Unfortunately I cannot change this as this logic persists
    // throughout everything this string buffer is used in.
    buffer->string =
        reinterpret_cast<char*>(buffer->string + (padded_len * 2) + 1);

    // Fix the function
    *reinterpret_cast<u32*>(0x803b7f38) = 0x803b7e9c;
    return dest;
}

ROOM_DEFINE_PATCH = {
    Patch::WriteFunctionCall(0x8003dc74, EncryptAndCopyStringToBuffer)};
} // namespace room::Encryption