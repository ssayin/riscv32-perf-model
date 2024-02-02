// SPDX-FileCopyrightText: 2023 Serdar Sayın <https://serdarsayin.com>
//
// SPDX-License-Identifier: Apache-2.0

#ifndef MODEL_CORE_H
#define MODEL_CORE_H

#include <memory>
#include <string>
#include <vector>

#include "BasicClock.h"
#include "BasicClockSubscriber.h"
#include "model/BasicClockSubscriber.h"
#include "model/IClock.h"
#include "model/ICore.h"

namespace model {

class Core : public ICore, public BasicClockSubscriber {
public:
  Core(std::string id, std::shared_ptr<BasicClock> clock)
      : BasicClockSubscriber(clock.get()), id{id}, clk{clock} {}

  void onPosEdge() override;
  void onNegEdge() override;
  void onAdvance() override;

  const PerfStats &getStats() const override;

private:
  std::shared_ptr<BasicClock> clk;
  std::string id;
  PerfStats stats;
  uint32_t instrPointer;
};

} // namespace model

#endif /* end of include guard: MODEL_CORE_H */
