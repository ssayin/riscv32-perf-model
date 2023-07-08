// SPDX-FileCopyrightText: 2023 Serdar Sayın <https://serdarsayin.com>
//
// SPDX-License-Identifier: Apache-2.0

#ifndef MODEL_CORE_H
#define MODEL_CORE_H

#include <memory>
#include <string>

#include "IClock.h"
#include "ICore.h"
#include "model/BasicClockSubscriber.h"

namespace model {

class Core : public ICore, public BasicClockSubscriber {
public:
  Core(std::string id, std::shared_ptr<BasicClock> clock)
      : clk{clock}, BasicClockSubscriber(clock.get()) {}

  void advance() override{};

  void onPosEdge() override {}
  void onNegEdge() override { stats.Cycles++; }

  const PerfStats &getStats() const override { return stats; }

private:
  std::shared_ptr<BasicClock> clk;
  PerfStats stats;
};

} // namespace model

#endif /* end of include guard: MODEL_CORE_H */
