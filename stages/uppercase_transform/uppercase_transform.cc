#include "flowpipe/stage.h"
#include "flowpipe/configurable_stage.h"
#include "flowpipe/observability/logging.h"
#include "flowpipe/plugin.h"

#include "uppercase_transform.pb.h"

#include <google/protobuf/struct.pb.h>
#include <google/protobuf/util/json_util.h>

#include <string>

using namespace flowpipe;

using UppercaseTransformConfig =
    flowpipe::stages::uppercase::v1::UppercaseTransformConfig;

// ============================================================
// UppercaseTransform
// (noop-style configurable transform)
// ============================================================
class UppercaseTransform final
    : public ITransformStage
    , public ConfigurableStage {
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

  // ------------------------------------------------------------
  // ConfigurableStage
  // ------------------------------------------------------------
  bool Configure(const google::protobuf::Struct& config) override {
    std::string json;
    auto status =
        google::protobuf::util::MessageToJsonString(config, &json);

    if (!status.ok()) {
      FP_LOG_ERROR("uppercase_transform failed to serialize config");
      return false;
    }

    UppercaseTransformConfig cfg;
    status =
        google::protobuf::util::JsonStringToMessage(json, &cfg);

    if (!status.ok()) {
      FP_LOG_ERROR("uppercase_transform invalid config");
      return false;
    }

    config_ = std::move(cfg);

    FP_LOG_INFO("uppercase_transform configured");

    if (config_.verbose()) {
      FP_LOG_INFO("uppercase_transform verbose logging enabled");
    }

    return true;
  }

  // ------------------------------------------------------------
  // ITransformStage
  // ------------------------------------------------------------
  void process(StageContext& ctx,
               const Payload& input,
               Payload& output) override {
    if (ctx.stop.stop_requested()) {
      return;
    }

    if (config_.verbose()) {
      FP_LOG_DEBUG("uppercase_transform processing payload");
    }

    // NOTE:
    // Payload is a non-owning view (const uint8_t*).
    // Real uppercasing requires owning/mutable buffers.
    // For now, behave as a pass-through like noop_transform.
    output = input;
  }

private:
  UppercaseTransformConfig config_{};
};

// ============================================================
// Plugin entry points
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
