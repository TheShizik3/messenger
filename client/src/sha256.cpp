#include "sha256.h"
#include <openssl/sha.h>
#include <sstream>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <iomanip>
#include <memory>
#include <stdexcept>

std::string sha256::hash(const std::string input) {
    EVP_MD_CTX* ctx = EVP_MD_CTX_new();
    if (!ctx) {
        throw std::runtime_error("EVP_MD_CTX_new failed");
    }

    auto cleanup = [](EVP_MD_CTX* ctx) { EVP_MD_CTX_free(ctx); };
    std::unique_ptr<EVP_MD_CTX, decltype(cleanup)> ctx_guard(ctx, cleanup);

    unsigned char digest[EVP_MAX_MD_SIZE];
    unsigned int digest_len = 0;

    EVP_DigestInit_ex(ctx, EVP_sha256(), nullptr);
    EVP_DigestUpdate(ctx, input.c_str(), input.length());
    EVP_DigestFinal_ex(ctx, digest, &digest_len);
    std::stringstream ss;
    ss << std::hex << std::setfill('0');
    
    for (unsigned int i = 0; i < digest_len; ++i) {
        ss << std::setw(2) << static_cast<unsigned int>(digest[i]);
    }
    
    return ss.str();
}
