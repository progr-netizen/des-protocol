#include "des.h"
#include <iostream>
#include <iomanip>
#include <sstream>

using des::u64;
using des::encrypt_block;
using des::decrypt_block;

static u64 parse_hex64(const std::string& s) {
  unsigned long long x = 0ULL;
  std::stringstream ss;
  ss << std::hex << s;
  ss >> x;
  return (u64)x;
}

int main(int argc, char** argv) {
  if (argc < 3) {
    std::cout << "Usage: des_cli <KEY64_HEX> <BLOCK64_HEX>\n";
    std::cout << "Example: des_cli 133457799BBCDFF1 0123456789ABCDEF\n";
    return 0;
  }
  u64 K = parse_hex64(argv[1]);
  u64 P = parse_hex64(argv[2]);
  u64 C = encrypt_block(P, K);
  std::cout << std::hex << std::uppercase << C << "\n";
  return 0;
}
