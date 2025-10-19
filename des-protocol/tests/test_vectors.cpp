#include "../src/des.h"
#include <iostream>
#include <iomanip>
#include <cassert>
#include <cstdio>
#include <cctype>

using des::u64;
using des::encrypt_block;
using des::decrypt_block;

static u64 hex64(const char* s) {
  unsigned long long x = 0ULL;
  std::sscanf(s, "%llx", &x);
  return (u64)x;
}

int main() {
  // NIST классика:
  // K = 133457799BBCDFF1, P = 0123456789ABCDEF, C = 85E813540F0AB405
  u64 K = hex64("133457799BBCDFF1");
  u64 P = hex64("0123456789ABCDEF");
  u64 Cexp = hex64("85E813540F0AB405");

  u64 C = encrypt_block(P, K);
  u64 P2 = decrypt_block(Cexp, K);

  bool ok1 = (C == Cexp);
  bool ok2 = (P2 == P);

  std::cout << std::hex << std::uppercase;
  std::cout << "Enc got: 0x" << C << " expected: 0x" << Cexp << (ok1? " [OK]":" [FAIL]") << "\n";
  std::cout << "Dec got: 0x" << P2 << " expected: 0x" << P << (ok2? " [OK]":" [FAIL]") << "\n";

  return (ok1 && ok2) ? 0 : 1;
}
