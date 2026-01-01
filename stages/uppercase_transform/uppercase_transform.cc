#include <algorithm>
#include <cctype>

#include "flowpipe/stage.h"
#include "flowpipe/plugin.h"

// Logging (plugin-safe)
#include "flowpipe/observability/logging.h"

using namespace flowpipe;

// ============================================================
// UppercaseTransform
// Converts payload bytes to uppercase
// ============================================================

class UppercaseTransform final : public ITransformStage {
public:
  std::string name() const override {
    return "uppercase_transform";
  }

  UppercaseTransform() {
    FP_LOG_INFO("uppercase_transform constructed");
  }

  ~UppercaseTransform() override {
    FP_LOG_INFO("uppercase_transform destroyed");
  }

  // ----------------------------------------------------------
  // Process a single payload
  // ----------------------------------------------------------
  void process(StageContext& ctx,
               const Payload& input,
               Payload& output) override {
    if (ctx.stop.stop_requested()) {
      FP_LOG_DEBUG("uppercase_transform stop requested");
      return;
    }

    FP_LOG_DEBUG("uppercase_transform processing payload");

    // Copy input → output
    output = input;

    // // Uppercase in-place (ASCII-safe)
    // std::transform(
    //     output.begin(),
    //     output.end(),
    //     output.begin(),
    //     [](unsigned char c) {
    //       return static_cast<unsigned char>(std::toupper(c));
    //     });
  }
};

// ============================================================
// Plugin entry points (EXPORTED)
// ============================================================

extern "C" {

  FLOWPIPE_PLUGIN_API
  IStage* flowpipe_create_stage() {
    FP_LOG_INFO("creating uppercase_transform stage");
    return new UppercaseTransform();
  }

  FLOWPIPE_PLUGIN_API
  void flowpipe_destroy_stage(IStage* stage) {
    FP_LOG_INFO("destroying uppercase_transform stage");
    delete stage;
  }

}  // extern "C"
