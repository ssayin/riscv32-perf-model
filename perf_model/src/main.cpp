#include "common/common.hpp"
#include "nlohmann/json.hpp"
#include "spdlog/spdlog.h"

#include "model/AtomicClock.h"
#include "model/Core.h"

int main(int argc, char **argv) {
  spdlog::info("Begin simulation");

  auto clk = std::make_shared<model::AtomicClock>();

  model::Core core0{"core_0", clk};
  model::Core core1{"core_1", clk};

  core0.advance();
  core1.advance();

  spdlog::info("End simulation");

  return 0;
}
