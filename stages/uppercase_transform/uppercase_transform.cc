#include "flowpipe/stage.h"
#include "flowpipe/configurable_stage.h"
#include "flowpipe/observability/logging.h"
#include "flowpipe/plugin.h"

#include "uppercase_transform.pb.h"

#include <google/protobuf/struct.pb.h>
#include <google/protobuf/util/json_util.h>

#include <cstdlib>
#include <cstring>

#include "uppercase_transform_util.h"

using namespace flowpipe;

using UppercaseTransformConfig =
    flowpipe::stages::uppercase::v1::UppercaseTransformConfig;

// ============================================================
// UppercaseTransform
// ============================================================
class UppercaseTransform final
    : public ITransformStage,
      public ConfigurableStage {
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

    if (input.empty()) {
      output = input;
      return;
    }

    if (config_.verbose()) {
      FP_LOG_DEBUG("uppercase_transform processing payload");
    }

    const size_t size = input.size;

    // ----------------------------------------------------------
    // Allocate new payload
    // ----------------------------------------------------------
    auto buffer = AllocatePayloadBuffer(size);
    if (!buffer) {
      FP_LOG_ERROR("uppercase_transform failed to allocate payload");
      return;
    }

    const uint8_t* src = input.data();
    uint8_t* dst = static_cast<uint8_t*>(buffer.get());

    // ----------------------------------------------------------
    // Uppercase transform (ASCII)
    // ----------------------------------------------------------
    flowpipe::stages::uppercase::UppercaseAscii(src, dst, size);

    // log if verbose
    if (config_.verbose()) {
      std::cout << dst << std::endl;
    }

    // build output
    output = Payload(std::move(buffer), size);
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
