#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <sstream>
#include <array>

std::string getSwaymsgOutput() {
  std::array<char, 128> buffer;
  std::string result;
  std::unique_ptr<FILE, decltype(&pclose)> pipe(popen("swaymsg -t get_outputs", "r"), pclose);
  if (!pipe) {
    throw std::runtime_error("popen() failed");
  }
  while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
    result += buffer.data();
  }
  return result;
}

std::string parseSwaymsgOutput(std::string swayOut) {
  std::istringstream iss(swayOut);
  for (std::string line; std::getline(iss, line); ) {
    for (int i = 0; i < line.length(); ++i) {
      if (line.substr(i, 22) == std::string("\"current_workspace\": \"")) {
        i = i + 22;
        for (int ii = 0; i + ii < line.length(); ++ii) {
          if (line.substr(i + ii, 1) == std::string("\"")) {
            return line.substr(i, ii);
          }
        }
      }
    }
  }
  throw std::runtime_error("Could not find active workspace");
}

int main() {
  std::string swayOut = getSwaymsgOutput();
  std::string out = parseSwaymsgOutput(swayOut);
  std::cout << out << std::endl;
}
