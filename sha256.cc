#include "log.h"

#include <iomanip>
#include <openssl/sha.h>
#include <sstream>
#include <string>
#include <vector>

std::string GetSHA256(const std::string str) {
  unsigned char hash[SHA256_DIGEST_LENGTH];
  std::vector<unsigned char> str_uc;
  for (const char &c : str) {
    str_uc.push_back(c);
  }

  unsigned char *r = SHA256(str_uc.data(), str.size(), hash);
  CHECK(r != nullptr);

  std::stringstream ss;

  for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
    ss << std::hex << std::setw(2) << std::setfill('0')
       << static_cast<int>(hash[i]);
  }
  return ss.str();
}
