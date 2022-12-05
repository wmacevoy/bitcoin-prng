#include "gtest/gtest.h"
#include <string>
#include <vector>
#include <utility>

#include "sha2.h"

std::vector < std::pair<std::string, std::string> > test1_vectors =
  {
   {"ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad","abc"},
   {"e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855",""},
   {"248d6a61d20638b8e5c026930c3e6039a33ce45964ff2167f6ecedd419db06c1","abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq"}
  };

TEST(Sha2,Vectors) {
  for (int i=0; i<test1_vectors.size(); ++i) {
    std::string expect = test1_vectors[i].first;
    std::string input = test1_vectors[i].second;
    
    sha2::hash<256> h;
    std::string result = h.update(input).final().hex();
    ASSERT_EQ(expect,result) << " for input " << input;;
  }
}

TEST(Sha2,PartialUpdate) {
  for (int i=0; i<test1_vectors.size(); ++i) {
    std::string expect = test1_vectors[i].first;
    std::string input = test1_vectors[i].second;
    for (int j=0; j<=input.size(); ++j) {
      std::string a = input.substr(0,j);
      std::string b = input.substr(j);
      ASSERT_EQ(input,a+b);
      sha2::hash<256> h;
      std::string result = h.update(a).update(b).final().hex();
      ASSERT_EQ(expect,result) << "for '" << a << "' + '" << b << "'";
    }
  }
}

