/*
 * Updated to C++, zedwood.com 2012
 * Based on Olivier Gay's version
 * See Modified BSD License below: 
 *
 * FIPS 180-2 SHA-224/256/384/512 implementation
 * Issue date:  04/30/2005
 * http://www.ouah.org/ogay/sha2/
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
#include <string>
#include <array>
#include <stdint.h>

#include "sha2.h"
 
class SHA256
{
protected:
  static const uint32_t sha256_k[];
  static const uint32_t SHA224_256_BLOCK_SIZE = (512/8);
public:
  static const uint32_t DIGEST_SIZE = ( 256 / 8);
  
  struct digest_type : std::array<uint8_t,DIGEST_SIZE> {
    std::string hex() const;
    void zero();
    digest_type& operator+=(const digest_type &digest);
    ~digest_type();
  };

  SHA256();
  
  SHA256& reset();
  SHA256& update(const uint8_t *message, std::size_t len);
  SHA256& update(const char *message, std::size_t len=std::string::npos);
  template <typename T>
  SHA256& update(const std::basic_string<T> &message) {
    if (message.length() == 0) return *this;
    return update((const uint8_t*)&message[0],message.length()*sizeof(T));
  }

  template <typename T, std::size_t N>
  SHA256& update(const std::array<T,N> &message) {
    if (N == 0) return *this;
    return update((const uint8_t*)&message[0],N*sizeof(T));
  }

  SHA256& digest(std::array<uint8_t,DIGEST_SIZE> &ans);

  digest_type digest();

protected:
  SHA256& transform(const uint8_t *message, std::size_t block_nb);
  std::size_t m_tot_len;
  std::size_t m_len;
  std::array < uint8_t, 2*SHA224_256_BLOCK_SIZE > m_block;
  std::array < uint32_t, 8 > m_h;
};

template <typename T> SHA256& operator<<(SHA256& ctx, const T &t) {
  return ctx.update(t);
}

#define sha256(x) ([&](){SHA256 ctx; return (ctx << x).digest(); }())
