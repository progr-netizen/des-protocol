#pragma once

// === TODO(Agent): заполнить таблицы DES из стандарта (1-индексные) ===

// Initial Permutation (IP)
static const int IP[64] = {
  // TODO
};

// Final Permutation (FP) — обратная к IP
static const int FP[64] = {
  // TODO
};

// Expansion E (32 -> 48)
static const int E[48] = {
  // TODO
};

// Straight Permutation P (32)
static const int P[32] = {
  // TODO
};

// PC-1 (Parity drop) (64 -> 56)
static const int PC1[56] = {
  // TODO
};

// PC-2 (Compression) (56 -> 48)
static const int PC2[48] = {
  // TODO
};

// Left shifts per round
static const int SHIFTS[16] = {
  // Обычно: 1,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1
  // TODO
};

// 8 S-boxes: S[box][row][col]
static const int S[8][4][16] = {
  // TODO: S1..S8
};
