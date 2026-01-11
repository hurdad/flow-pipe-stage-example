#include "uppercase_transform_util.h"

#include <cctype>

namespace flowpipe::stages::uppercase {

void UppercaseAscii(const uint8_t* src, uint8_t* dst, size_t size) {
  for (size_t i = 0; i < size; ++i) {
    dst[i] = static_cast<uint8_t>(
        std::toupper(static_cast<unsigned char>(src[i])));
  }
}

}  // namespace flowpipe::stages::uppercase
