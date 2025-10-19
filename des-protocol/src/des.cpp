#include "des.h"
#include "tables.h"
#include <cassert>
#include <cstdio>
#include <cinttypes>

namespace des {
using namespace detail;

// Generic permutation helper (1-indexed tables)
template<size_t OUTN, size_t INN>
static uint64_t permute_u(uint64_t in, const int* table) {
  uint64_t out = 0;
  for (size_t i = 0; i < OUTN; ++i) {
    int src = table[i] - 1;
    uint64_t bit = (in >> (INN - 1 - src)) & 1ULL;
    out |= (bit << (OUTN - 1 - i));
  }
  return out;
}

Block encrypt_block(Block pt, Key64 k) {
  auto rk = key_schedule(k);
  Block x = ip(pt);

  uint32_t L = (uint32_t)(x >> 32);
  uint32_t R = (uint32_t)(x & 0xFFFFFFFFu);

  for (int i = 0; i < 16; ++i) {
    uint32_t tmp = R;
    uint32_t f = feistel(R, rk[i]);
    R = L ^ f;
    L = tmp;
  }

  uint64_t pre = ( (uint64_t)R << 32 ) | L; // swap
  return fp(pre);
}

Block decrypt_block(Block ct, Key64 k) {
  auto rk = key_schedule(k);
  Block x = ip(ct);

  uint32_t L = (uint32_t)(x >> 32);
  uint32_t R = (uint32_t)(x & 0xFFFFFFFFu);

  for (int i = 0; i < 16; ++i) {
    uint32_t tmp = R;
    uint32_t f = feistel(R, rk[15 - i]);
    R = L ^ f;
    L = tmp;
  }

  uint64_t pre = ( (uint64_t)R << 32 ) | L;
  return fp(pre);
}

namespace detail {

Block ip(Block x) {
  // TODO(Agent): Implement Initial Permutation using IP[64].
  return x;
}
Block fp(Block x) {
  // TODO(Agent): Implement Final Permutation using FP[64].
  return x;
}

std::array<u64,16> key_schedule(Key64 key64) {
  std::array<u64,16> rks{};
  // TODO(Agent): PC1 (64->56), split C,D (28/28), left shifts per SHIFTS, PC2 (56->48)
  // Pack round key in lower 48 bits of u64.
  (void)key64;
  return rks;
}

uint32_t feistel(uint32_t r, u64 rk) {
  // TODO(Agent):
  // 1) Expansion E (32->48)
  // 2) XOR with rk (48-bit)
  // 3) Split into 8 chunks of 6 bits → S-box lookup (row by bits 1&6, col by 2..5)
  // 4) Concatenate 8×4 bits → 32-bit → Straight P permutation
  (void)rk;
  return r;
}

} // namespace detail
} // namespace des
