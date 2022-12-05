#pragma once

#include "sodium.h"

namespace sha2 {
  static const char *const hexcodes="0123456789abcdef";
  
  template <std::uint32_t bits>
    struct digest : std::array < std::uint8_t , (bits+7)/8 > {
    static constexpr std::uint32_t size { (bits+7)/8 };
    std::string hex() const {
      std::string codes(2*size,'0');
      for (int i = 0; i < size; i++) {
	codes[2*i+0]=hexcodes[0xF&((*this)[i]>>4)];
	codes[2*i+1]=hexcodes[0xF&((*this)[i]>>0)];
      }
      return codes;
    }
    void zero() { memset(&(*this)[0],0,size); }
    ~digest() { zero(); }
  };

  template <std::uint32_t bits>
    struct hash;

  template <>
    struct hash<256> {
    static constexpr std::uint32_t BITS { 256 };
    static constexpr std::uint32_t DIGEST_SIZE { BITS/8 };
    static constexpr std::uint32_t BLOCK_SIZE { BITS/4 };
    crypto_hash_sha256_state state;
    
    hash& init() { crypto_hash_sha256_init(&state); return *this; }
    hash& zero() { memset(&state,0,sizeof(state)); return *this; }
    hash& update(const void *message, std::uint64_t len) {
      crypto_hash_sha256_update(&state, (const unsigned char*) message, len);
      return *this;
    }
    hash& update(const std::string &message) {
      return update(&message[0],message.length());
    }
    hash& final(void *digest) {
      crypto_hash_sha256_final(&state, (unsigned char*)digest);
      return *this;
    }
    hash& final(digest<BITS> &_digest) {
      return final(&_digest[0]);
    }
    
    digest<BITS> final() {
      digest<BITS> ans;
      final(ans);
      return ans;
    }

    hash() { init(); }
    ~hash() { zero(); }
  };

  template <>
    struct hash<512> {
    static constexpr std::uint32_t BITS { 512 };
    static constexpr std::uint32_t DIGEST_SIZE { 512/8 };
    static constexpr std::uint32_t BLOCK_SIZE { 512/4 };
    crypto_hash_sha512_state state;
    
    hash& init() { crypto_hash_sha512_init(&state); return *this; }
    hash& zero() { memset(&state,0,sizeof(state)); return *this; }
    hash& update(const void *message, std::uint64_t len) {
      crypto_hash_sha512_update(&state, (const unsigned char*) message, len);
      return *this;
    }
    hash& update(const std::string &message) {
      return update(&message[0],message.length());
    }

    hash& final(void *digest) {
      crypto_hash_sha512_final(&state, (unsigned char*) digest);
      return *this;
    }
    hash& final(digest<BITS> &_digest) {
      return final(&_digest[0]);
    }
    
    digest<BITS> final() {
      digest<BITS> ans;
      final(ans);
      return ans;
    }
    hash() { init(); }
    ~hash() { zero(); }
  };

} // namespace sha2
