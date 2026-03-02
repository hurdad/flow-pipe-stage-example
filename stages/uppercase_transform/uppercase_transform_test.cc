#include "flowpipe/configurable_stage.h"
#include "flowpipe/stage.h"

#include <google/protobuf/struct.pb.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>

extern "C" flowpipe::IStage* flowpipe_create_stage();
extern "C" void flowpipe_destroy_stage(flowpipe::IStage* stage);

namespace {

void AssertTrue(bool condition, const char* message) {
  if (!condition) {
    std::fprintf(stderr, "%s\n", message);
    std::exit(1);
  }
}

void AssertEqual(const std::string& expected, const std::string& actual) {
  if (expected != actual) {
    std::fprintf(stderr,
                 "Expected '%s' but got '%s'\n",
                 expected.c_str(),
                 actual.c_str());
    std::exit(1);
  }
}

flowpipe::Payload PayloadFromString(const std::string& value) {
  auto buffer = flowpipe::AllocatePayloadBuffer(value.size());
  AssertTrue(static_cast<bool>(buffer), "Failed to allocate test payload buffer");
  std::memcpy(buffer.get(), value.data(), value.size());
  return flowpipe::Payload(std::move(buffer), value.size());
}

std::string PayloadToString(const flowpipe::Payload& payload) {
  return std::string(reinterpret_cast<const char*>(payload.data()), payload.size);
}

}  // namespace

int main() {
  flowpipe::IStage* stage = flowpipe_create_stage();
  AssertTrue(stage != nullptr, "Failed to create stage");

  auto* configurable = dynamic_cast<flowpipe::ConfigurableStage*>(stage);
  AssertTrue(configurable != nullptr, "Stage is not configurable");

  google::protobuf::Struct config;
  (*config.mutable_fields())["verbose"].mutable_bool_value()->set_value(false);
  AssertTrue(configurable->configure(config), "Stage configure() failed");

  auto* transform = dynamic_cast<flowpipe::ITransformStage*>(stage);
  AssertTrue(transform != nullptr, "Stage is not a transform");

  flowpipe::StageContext ctx;
  const auto input = PayloadFromString("Hello, Flow-Pipe!");
  flowpipe::Payload output;

  transform->process(ctx, input, output);
  AssertEqual("HELLO, FLOW-PIPE!", PayloadToString(output));

  flowpipe_destroy_stage(stage);
  return 0;
}
