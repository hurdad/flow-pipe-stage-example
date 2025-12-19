#include <algorithm>

#include "flowpipe/stage.h"

using namespace flowpipe;

class UppercaseTransform final : public ITransformStage {
public:
  std::string name() const override {
    return "uppercase_transform";
  }

  void run(StageContext& ctx,
          BoundedQueue<Payload>& in,
          BoundedQueue<Payload>& out) override {
    while (!ctx.stop.stop_requested()) {
      auto item = in.pop(ctx.stop);
      if (!item.has_value()) {
        break;
      }

      std::transform(
          item.value().begin(),
          item.value().end(),
          item.value().begin(),
          [](unsigned char c) { return std::toupper(c); });


      if (!out.push(std::move(*item), ctx.stop)) {
        break;
      }
    }
  }
};


extern "C" {

  IStage* flowpipe_create_stage() {
    return new UppercaseTransform();
  }

  void flowpipe_destroy_stage(IStage* stage) {
    delete stage;
  }

}
