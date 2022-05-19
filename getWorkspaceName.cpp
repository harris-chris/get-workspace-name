#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <sstream>
#include <array>

std::string getWMCtrlOutput() {
  std::array<char, 128> buffer;
  std::string result;
  std::unique_ptr<FILE, decltype(&pclose)> pipe(popen("wmctrl -d", "r"), pclose);
  if (!pipe) {
    throw std::runtime_error("popen() failed");
  }
  while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
    result += buffer.data();
  }
  return result;
}

std::string parseWMCtrlOutput(std::string wmCtrlOut) {
  std::istringstream iss(wmCtrlOut);
  bool activeWS {false};
  int tabsCounted = 0;

  for (std::string line; std::getline(iss, line); ) {
    for (int i = 0; i < line.length(); ++i) {
      if (line[i] == '*') {
        activeWS = true;
      }
      if (activeWS) {
        if (line.substr(i, 2) == std::string("  ")) {
          ++tabsCounted;
        }
        if (tabsCounted == 3) {
          return line.substr(i+2, line.length());
        }
      }
    }
  }
  throw std::runtime_error("Could not find active workspace");
}

int main() {
  std::string wmCtrlOut = getWMCtrlOutput();
  std::string out = parseWMCtrlOutput(wmCtrlOut);
  std::cout << out << std::endl;
}
