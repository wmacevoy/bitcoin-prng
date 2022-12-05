#include "gtest/gtest.h"
#include <string>
#include <vector>
#include <utility>

#include "sha256.h"

std::vector < std::pair<std::string, std::string> > test1_vectors =
  {
   {"ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad","abc"},
   {"e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855",""},
   {"248d6a61d20638b8e5c026930c3e6039a33ce45964ff2167f6ecedd419db06c1","abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq"}
  };

TEST(Sha256,Vectors) {
  for (int i=0; i<test1_vectors.size(); ++i) {
    std::string expect = test1_vectors[i].first;
    std::string input = test1_vectors[i].second;
    
    SHA256 h;
    std::string result = h.update(input).digest().hex();
    ASSERT_EQ(expect,result) << " for input " << input;;
  }
}

TEST(Sha256,PartialUpdate) {
  for (int i=0; i<test1_vectors.size(); ++i) {
    std::string expect = test1_vectors[i].first;
    std::string input = test1_vectors[i].second;
    for (int j=0; j<=input.size(); ++j) {
      std::string a = input.substr(0,j);
      std::string b = input.substr(j);
      ASSERT_EQ(input,a+b);
      SHA256 h;
      std::string result = h.update(a).update(b).digest().hex();
      ASSERT_EQ(expect,result) << "for '" << a << "' + '" << b << "'";
    }
  }
}

TEST(Sha256,Macro) {
  for (int i=0; i<test1_vectors.size(); ++i) {
    std::string expect = test1_vectors[i].first;
    std::string input = test1_vectors[i].second;
    for (int j=0; j<=input.size(); ++j) {
      std::string a = input.substr(0,j);
      std::string b = input.substr(j);
      std::string result = sha256(a << b).hex();
      ASSERT_EQ(expect,result) << "for '" << a << "' + '" << b << "'";
    }
  }
}


std::vector < std::tuple<std::string,std::string,std::string> > test2_vectors =
  {
   {"d7b8a31c5ed1dbc241b10676e1aec94b58c8bb7816c052036bdcc0ba44511f9a","","abc"},
   {"a9dc76795943c6cbb64968bb8628cfe4d0bf6e7967eb01f44421c9b2c047d874","a","bc"},
   {"3e894f2c2bdebd0e37b1bc2bb4345690b544d8efc9d9002e46cb898ad9018630","ab","c"},
   {"1de51655e1a8ca5f570c062c6fef497dd8110762bd8a7a48dd95482f7d22e0a2","abc",""},
  };

TEST(Sha256,PartialDigestUpdate) {
  for (int i=0; i<test2_vectors.size(); ++i) {
    std::string expect = std::get<0>(test2_vectors[i]);
    std::string a = std::get<1>(test2_vectors[i]);
    std::string b = std::get<2>(test2_vectors[i]);
    SHA256 h;
    std::string part = h.update(a).digest().hex();
    std::string result = h.update(b).digest().hex();
    ASSERT_EQ(expect,result) << "for '" << a << "' + '" << b << "'";
  }
}

TEST(Sha256,Stream) {
  for (int i=0; i<test2_vectors.size(); ++i) {
    std::string expect = std::get<0>(test2_vectors[i]);
    std::string a = std::get<1>(test2_vectors[i]);
    std::string b = std::get<2>(test2_vectors[i]);
    SHA256 h;
    std::string part = h.update(a).digest().hex();
    std::string result = h.update(b).digest().hex();
    ASSERT_EQ(expect,result) << "for '" << a << "' + '" << b << "'";
  }
}

