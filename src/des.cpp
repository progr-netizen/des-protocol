#include "des.h"
#include "tables.h"
#include <cassert>
#include <cinttypes>

namespace des {
using namespace detail;

// generic permutation (1-indexed)
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

  uint64_t pre = ((uint64_t)R << 32) | L; // swap
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

  uint64_t pre = ((uint64_t)R << 32) | L;
  return fp(pre);
}

namespace detail {

Block ip(Block x) { return (Block)permute_u<64,64>(x, IP); }
Block fp(Block x) { return (Block)permute_u<64,64>(x, FP); }

// helper: left-rotate 28-bit value by s
static inline uint32_t rol28(uint32_t v, int s) {
  v &= 0x0FFFFFFFu;
  return ((v << s) | (v >> (28 - s))) & 0x0FFFFFFFu;
}

std::array<u64,16> key_schedule(Key64 key64) {
  std::array<u64,16> rks{};

  // PC1: 64 -> 56
  uint64_t K56 = permute_u<56,64>(key64, PC1);

  // split C,D
  uint32_t C = (uint32_t)((K56 >> 28) & 0x0FFFFFFFu);
  uint32_t D = (uint32_t)(K56 & 0x0FFFFFFFu);

  for (int i = 0; i < 16; ++i) {
    C = rol28(C, SHIFTS[i]);
    D = rol28(D, SHIFTS[i]);

    uint64_t CD = ((uint64_t)C << 28) | (uint64_t)D; // 56 bits
    uint64_t K48 = permute_u<48,56>(CD, PC2);
    rks[i] = K48 & 0xFFFFFFFFFFFFULL; // store in low 48 bits
  }
  return rks;
}

uint32_t feistel(uint32_t r, u64 rk) {
  // E: 32 -> 48
  uint64_t R48 = permute_u<48,32>(r, E);
  R48 ^= (rk & 0xFFFFFFFFFFFFULL);

  // S-boxes: 8 * 6 -> 8 * 4 = 32
  uint32_t out32 = 0;
  for (int b = 0; b < 8; ++b) {
    int shift = (7 - b) * 6;
    uint8_t chunk = (R48 >> shift) & 0x3F;

    int row = ((chunk & 0x20) >> 4) | (chunk & 0x01); // bits 6 and 1
    int col = (chunk >> 1) & 0x0F;                   // bits 5..2

    uint8_t s = (uint8_t)S[b][row][col];
    out32 <<= 4;
    out32 |= (s & 0x0F);
  }

  // P permutation
  uint32_t p = (uint32_t)permute_u<32,32>(out32, P);
  return p;
}

} // namespace detail
} // namespace des
