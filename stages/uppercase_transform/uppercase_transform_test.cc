#include "uppercase_transform_util.h"

#include <cstdio>
#include <cstdint>
#include <cstdlib>
#include <string>

namespace {

void AssertEqual(const std::string& expected, const std::string& actual) {
  if (expected != actual) {
    std::fprintf(stderr,
                 "Expected '%s' but got '%s'\n",
                 expected.c_str(),
                 actual.c_str());
    std::exit(1);
  }
}

}  // namespace

int main() {
  {
    const std::string input = "Hello, Flow-Pipe!";
    std::string output(input.size(), '\0');

    flowpipe::stages::uppercase::UppercaseAscii(
        reinterpret_cast<const uint8_t*>(input.data()),
        reinterpret_cast<uint8_t*>(output.data()),
        input.size());

    AssertEqual("HELLO, FLOW-PIPE!", output);
  }

  {
    const std::string input = "already upper";
    std::string output(input.size(), '\0');

    flowpipe::stages::uppercase::UppercaseAscii(
        reinterpret_cast<const uint8_t*>(input.data()),
        reinterpret_cast<uint8_t*>(output.data()),
        input.size());

    AssertEqual("ALREADY UPPER", output);
  }

  {
    const std::string input;
    std::string output;

    flowpipe::stages::uppercase::UppercaseAscii(nullptr, nullptr, 0);

    AssertEqual(input, output);
  }

  return 0;
}
