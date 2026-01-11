#ifndef FLOWPIPE_STAGES_UPPERCASE_TRANSFORM_UTIL_H_
#define FLOWPIPE_STAGES_UPPERCASE_TRANSFORM_UTIL_H_

#include <cstddef>
#include <cstdint>

namespace flowpipe::stages::uppercase {

void UppercaseAscii(const uint8_t* src, uint8_t* dst, size_t size);

}  // namespace flowpipe::stages::uppercase

#endif  // FLOWPIPE_STAGES_UPPERCASE_TRANSFORM_UTIL_H_
