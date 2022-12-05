// verifiable?
//
// count = 65000*600*24 = 936 000 000  = 1 billion
// save 1'000'000 step actions (VDF) => 1000 states
// DB -> a -> return a
// Block(k) -> SHA512(Block(k)) -> SHA256(b512) -> pbkdf2_sha256(b512,512) -> SHA512(b512|dk512) -> store
// XOR SHA512(store(k) | store(top))
//                      const uint8_t *salt, size_t saltlen, uint64_t c,
//                      uint8_t *buf, size_t dkLen)
k-10
// https://medium.com/ginar-io/a-review-of-random-number-generator-rng-on-blockchain-fe342d76261b
// Verifiable Delay Functions
