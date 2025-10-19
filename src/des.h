#pragma once
#include <array>
#include <cstdint>
#include <vector>

namespace des {

using u64 = uint64_t;
using Block = u64;
using Key64 = u64;

Block encrypt_block(Block plaintext, Key64 key64);
Block decrypt_block(Block ciphertext, Key64 key64);

namespace detail {
  std::array<u64,16> key_schedule(Key64 key64);     // 16 round keys (48-bit packed into u64)
  uint32_t feistel(uint32_t r, u64 roundKey48);     // E→XOR→S→P
  Block ip(Block x);                                // Initial Permutation
  Block fp(Block x);                                // Final Permutation
}

} // namespace des
