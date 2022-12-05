#pragma once

#include <stdint.h>

template <typename H,uint32_t STATE_SIZE, uint32_t HASH_SIZE>
class Hash {
    private: std::array <uint64_t, (STATE_SIZE+63)/64> state;
    virtual void reset()=0;
    virtual void update(const uint8_t *data, uint32_t len)=0;
    void update(const Digest &digest) {
    }

    uint32_t size() const { return HASH_SIZE; }
    struct Digest {
        private:  std::array<uint64_t,(HASH_SIZE+63)/64> data;

        uint64_t u64(int k) const {
            return data.at(k);
        }
        void reset() {
            for (uint32_t i=0; i<(HASH_SIZE+63)/64; ++i) {
                data[i]=0;
            }
        }
        Digest& operator^= (const Digest &digest) {
            for (uint32_t i=0; i<(HASH_SIZE+63)/64; ++i) {
                data[i] ^= digest.data[i];
            }
        }
        Digest& operator= (uint64_t value) {
            reset();
            data[0]=value;
        }
    };
};

template < class Hash > 
class PRNG {
    private: Hash hash;

    public: void reset(const Hash::Digest &seed) {
        hash.reset();
        hash.update(seed);
    }

    public: void update(const Hash::Digest &data) {
        hash.update(data);
    }

    public: void update(const uint8_t *data, uint32_t len) {
        hash.update(data,len);
    }

    public: void value(uint64_t i, Hash::Digest &value) const {
        Hash iHash;
        uint8_t le[8];
        for (int b=0; b<8; ++b) {
            le[b]=(i & 0xFF);
            i >>= 8;
        }
        Hash::Digest digest;
        hash.digest(digest);
        iHash.reset(digest);
        iHash.update(le,8);
        iHash.digest(value);
    }
};


template < class Hash > 
class PRNG {
    private: Hash::Digest seed;
    private: Hash hash;

    public: void reset(const Hash::Digest &seed) {
        hash.reset();
        hash.update(seed);
    }

    public: void update(const Hash::Digest &data) {
        hash.update(data);
    }

    public: void update(const uint8_t *data, uint32_t len) {
        hash.update(data,len);
    }

    public: void value(uint64_t i, Hash::Digest &value) const {
        Hash iHash;
        uint8_t le[8];
        for (int b=0; b<8; ++b) {
            le[b]=(i & 0xFF);
            i >>= 8;
        }
        Hash::Digest digest;
        hash.digest(digest);
        iHash.reset(digest);
        iHash.update(le,8);
        iHash.digest(value);
    }
};

template < class Hash, uint8_t N >
class PooledPRNG {
    public: uint32_t _delay;
     PooledPRNG(uint32_t delay = 1'000'000) 
       : delay(_delay) {}
    private: std::array < PRNG < Hash >, N > pool;
    public: void reset(const Hash::Digest &seed) {
        pool[0].reset(seed);
        Hash::Digest digest;
        for (uint8_t i=1; i<N; ++i) {
            pool[]
            prng.value(i,iSeed);
            pool[i].reset(iSeed);
        }
    }

    public: void update(const Hash::Digest &data) {
        Hash::Digest digest;
        Hash::Digest zero;

        zero.reset();
        value(0xFFFF_FFFF_FFFF_FFFFul,digest);
        Hash hash;
        hash.update(data);

        for (uint8_t t=0; t<delay; ++t) {
            hash.digest(digest);
            hash.update(digest);
        }
        hash.digest(digest);

        uint64 d0=digest.u64(0);
        for (uint8_t i=0; i<N; ++i) {
            pool[i].update((d0 % (1ul<<i) == 0) ? digest : zero);
        }
    }

    public: void update(uint8_t *data, uint32_t len) {
        Hash hash;
        Hash::Digest digest;
        hash.reset();
        hash.update(data,len);
        hash.digest(digest);
        update(digest);
    }

    public: void value(uint64_t i, Hash::Digest &value) {
        Hash::Digest sum;
        Hash::Digest part;
        sum.reset();
        for (uint8_t j=0; j<N; ++j) {
            pool[j].value(i,part);
            sum ^= part;
        }
        value = sum;
    }
};
