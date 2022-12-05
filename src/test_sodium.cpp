#include "gtest/gtest.h"

#include "sodium.h"

TEST(Sodium,Sha256) {
  std::vector < std::pair<std::string, std::string> > test_vectors =
    {
     {"ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad","abc"},
     {"e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855",""},
     {"248d6a61d20638b8e5c026930c3e6039a33ce45964ff2167f6ecedd419db06c1","abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq"}
    };
  for (int i=0; i<test_vectors.size(); ++i) {
    std::string expect = test_vectors[i].first;
    std::string input = test_vectors[i].second;
    
    unsigned char out[crypto_hash_sha256_BYTES];
    char result[2*crypto_hash_sha256_BYTES+1];
    crypto_hash_sha256_state state;

    crypto_hash_sha256_init(&state);

    crypto_hash_sha256_update(&state, (const unsigned char*) &input[0], input.length());

    crypto_hash_sha256_final(&state, out);

    for (int i=0; i<crypto_hash_sha256_BYTES; ++i) {
      sprintf(result+2*i,"%02x",out[i]);
    }

    ASSERT_EQ(expect,result) << " for input " << input;;
  }
}
