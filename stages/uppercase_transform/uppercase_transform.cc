#include <algorithm>
#include <cctype>
#include <string>

#include "flow_pipe/stage.h"
#include "flow_pipe/record.h"

using namespace flow_pipe;

class UppercaseTransform final : public Stage {
public:
  UppercaseTransform(const StageContext& ctx)
      : Stage(ctx) {}

  void process(Record&& record) override {
    auto& value = record.value<std::string>();

    std::transform(
        value.begin(),
        value.end(),
        value.begin(),
        [](unsigned char c) { return std::toupper(c); });

    emit(std::move(record));
  }
};

extern "C" Stage* create_stage(const StageContext& ctx) {
  return new UppercaseTransform(ctx);
}

extern "C" void destroy_stage(Stage* stage) {
  delete stage;
}
