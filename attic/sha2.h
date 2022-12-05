/*
 * FIPS 180-2 SHA-224/256/384/512 implementation
 * Last update: 02/02/2007
 * Issue date:  04/30/2005
 *
 * Copyright (C) 2005, 2007 Olivier Gay <olivier.gay@a3.epfl.ch>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the project nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE PROJECT AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE PROJECT OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#pragma once

#include <cstdint>

namespace sha2 {

  constexpr std::uint32_t SHA224_DIGEST_SIZE { 224 / 8 };
  constexpr std::uint32_t SHA256_DIGEST_SIZE { 256 / 8 };
  constexpr std::uint32_t SHA384_DIGEST_SIZE { 384 / 8 };
  constexpr std::uint32_t SHA512_DIGEST_SIZE { 512 / 8 };
  
  constexpr std::uint32_t SHA256_BLOCK_SIZE  { 512 / 8 };
  constexpr std::uint32_t SHA512_BLOCK_SIZE  {1024 / 8 };
  constexpr std::uint32_t SHA384_BLOCK_SIZE  { SHA512_BLOCK_SIZE };
  constexpr std::uint32_t SHA224_BLOCK_SIZE  { SHA256_BLOCK_SIZE };

  struct sha256_ctx {
    std::uint64_t tot_len;
    std::uint64_t len;
    std::array<std::uint8_t,2*SHA256_BLOCK_SIZE> block;
    std::array<std::unit32_t,8> h;
  };

  struct sha512_ctx {
    std::uint64_t tot_len;
    std::uint64_t len;
    std::array<std::uint8_t,2*SHA512_BLOCK_SIZE> block;
    std::array<std::unit32_t,8> h;
  };

  typedef sha512_ctx sha384_ctx;
  typedef sha256_ctx sha224_ctx;

  void sha224_init(sha224_ctx *ctx);
  void sha224_update(sha224_ctx *ctx, const void *message, std::uint64_t len);
  void sha224_final(sha224_ctx *ctx, void *digest);
  void sha224(const void *message, std::uint64_t len, void *digest);

  void sha256_init(sha256_ctx *ctx);
  void sha256_update(sha256_ctx *ctx, const void *message, std::uint64_t len);
  void sha256_final(sha256_ctx *ctx, void *digest);
  void sha256(const void *message, std::uint64_t len, void *digest);
  
  void sha384_init(sha384_ctx *ctx);
  void sha384_update(sha384_ctx *ctx, const void *message, std::uint64_t len);
  void sha384_final(sha384_ctx *ctx, void *digest);
  void sha384(const void *message, std::uint64_t len, void *digest);

  void sha512_init(sha512_ctx *ctx);
  void sha512_update(sha512_ctx *ctx, const void *message, std::uint64_t len);
  void sha512_final(sha512_ctx *ctx, void *digest);
  void sha512(const void *message, std::uint64_t len, void *digest);

  template <std::uint32_t bits>
    struct hash;

  template <>
    struct hash<256> {
    using word = std::uint32_t;
    constexpr std::uint32_t DIGEST_SIZE { SHA256_DIGEST_SIZE };
    constexpr std::uint32_t BLOCK_SIZE { SHA256_BLOCK_SIZE };
    
    sha256_ctx ctx;
    
    void init() { sha256_init(&ctx); }
    void zero() { memset(&ctx,0,sizeof(ctx)); }
    void update(const void *message, std::uint64_t len) {
      sha256_update(&ctx,message,len);
    }
    void final(void *digest) {
      void sha256_final(&ctx,digest);
    }
    hash() { init(); }
    ~hash() { zero(); }
  };
} // namespace sha2
