#pragma once

#include <util.h>

namespace nhttp {
using ssl_ctx_t = u32;
using func = void (*)();

#pragma pack(push, 1)
    struct Self {
        u8 _[2020];
        func a_func;
    };

    struct Request {
        u8 _[40];
        char* hostname;
        u8 _1[128];
        ssl_ctx_t ssl_ctx;
        void* buffer1;
        u32 buffer1_len;
        void* buffer2;
        u32 buffer2_len;
        void* cert;
        u32 cert_len;
        u32 should_use_client_cert;
        u32 options;
        u32 unk;
        u32 a_field;
        u32 root_ca_index;
        u32 client_cert_index;
    };
#pragma pack(pop)

    LONGCALL ssl_ctx_t SSLNew(u32 options, const char* hostname) AT(0x803c0078);
    LONGCALL s32 SSLSetBuiltinClientCert(ssl_ctx_t ctx, u32 client_cert_index) AT(0x803c0cfc);
    LONGCALL s32 SSLSetClientCert(ssl_ctx_t ctx, void* buffer1, u32 buffer1_size, void* buffer2, u32 buffer2_size) AT(0x803c0980);
    LONGCALL s32 SSLSetRootCA(ssl_ctx_t ctx, const u8* cert, u32 cert_size) AT(0x803c0aec);
    LONGCALL s32 SSLConnect(ssl_ctx_t ctx, void* socket) AT(0x803c01d0);
    LONGCALL int NHTTPi_Request2Connection(void* _, nhttp::Request* request) AT(0x803bfb34);
    LONGCALL s32 SSLDoHandshake(ssl_ctx_t ctx) AT(0x803c0298);
    LONGCALL void NHTTPi_SetSSLError(nhttp::Self* self, s32 error) AT(0x803b80a4);
}