#include <cert.h>
#include <patch.h>
#include <ssl.h>

namespace room::NHTTP
{
s32 NHTTPi_SocSSLConnect(
    nhttp::Self* self, void* param_2, nhttp::Request* request, void* socket
)
{
    request->ssl_ctx = nhttp::SSLNew(request->options, request->hostname);
    if (self->a_func != nullptr && request->a_field != 0)
        self->a_func();

    if (request->should_use_client_cert == 1) {
        s32 res = nhttp::SSLSetBuiltinClientCert(
            request->ssl_ctx, request->client_cert_index
        );
        if (res != 0) {
            return -1005;
        }
    } else if (request->buffer1 != nullptr && request->buffer2 != nullptr) {
        s32 res = nhttp::SSLSetClientCert(
            request->ssl_ctx, request->buffer1, request->buffer1_len,
            request->buffer2, request->buffer2_len
        );
        if (res != 0) {
            return -1005;
        }
    }

    s32 ret = nhttp::SSLSetRootCA(
        request->ssl_ctx, wiilinkca_pub, wiilinkca_pub_size
    );
    if (ret != 0)
        return -1004;

    bool do_ret{};
    ret = nhttp::SSLConnect(request->ssl_ctx, socket);
    if (-2 < ret) {
        while (true) {
            if (do_ret) {
                return 0;
            }

            ret = nhttp::NHTTPi_Request2Connection(param_2, request);
            s32 err = nhttp::SSLDoHandshake(request->ssl_ctx);
            nhttp::NHTTPi_SetSSLError(self, err);
            if (ret != 0) {
                *reinterpret_cast<s32*>(ret + 8) = err;
            }

            if (err == -1) {
                break;
            }

            if (err < -1) {
                if (err != -7) {
                    if (err < -7) {
                        return -1001;
                    }
                    if (err < -3) {
                        return -1001;
                    }
                }
            } else {
                if (0 < err) {
                    return -1001;
                }
                do_ret = true;
            }
        }
    }

    return -1001;
}

ROOM_DEFINE_PATCH = {
    Patch::WriteFunctionCall(0x803ba388, NHTTPi_SocSSLConnect),
    Patch::WriteFunctionCall(0x803bce04, NHTTPi_SocSSLConnect)};
} // namespace room::NHTTP