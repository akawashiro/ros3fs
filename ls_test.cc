#include <cstdio>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>

std::vector<std::string> lsRoot() {
  using json = nlohmann::json;

  std::string exec_out = std::tmpnam(nullptr);
  std::string exec_cmd = std::string("docker exec -it ozone-instance "
                                     "./bin/ozone sh key ls /s3v/bucket1 > ") +
                         exec_out;

  // std::cout << "exec_cmd: " << exec_cmd << " exec_out: " << exec_out
  //           << std::endl;

  std::system(exec_cmd.c_str());

  std::ifstream f(exec_out);
  json data = json::parse(f);

  std::vector<std::string> result;
  for (json::iterator it = data.begin(); it != data.end(); ++it) {
    result.push_back((*it)["name"]);
  }
  return result;
}

int main() {
  const auto v = lsRoot();

  for (const auto &s : v) {
    std::cout << s << std::endl;
  }
}
