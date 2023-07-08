// SPDX-FileCopyrightText: 2023 Serdar Sayın <https://serdarsayin.com>
//
// SPDX-License-Identifier: Apache-2.0

#include "common/common.hpp"
#include "model/Model.h"
#include "nlohmann/json.hpp"
#include "spdlog/spdlog.h"

#include "model/BasicClock.h"
#include "model/Core.h"

void test() {
  auto clk = std::make_shared<model::BasicClock>();

  model::Core core0{"core_0", clk};
  model::Core core1{"core_1", clk};

  for (int i = 0; i < 3; ++i) {
    clk->advance();

    auto cycles_0 = core0.getStats().getTotalCycles();
    auto cycles_1 = core1.getStats().getTotalCycles();

    printf("Core0 Cycles: %d\n", cycles_0);
    printf("Core1 Cycles: %d\n", cycles_1);
  }
}
int main(int argc, char **argv) {
  spdlog::info("Begin simulation");

  auto clk = std::make_shared<model::BasicClock>();

  model::Model m{"model", clk};

  spdlog::info("End simulation");

  return 0;
}
