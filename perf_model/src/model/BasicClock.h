// SPDX-FileCopyrightText: 2023 Serdar Sayın <https://serdarsayin.com>
//
// SPDX-License-Identifier: Apache-2.0

#ifndef MODEL_BASICCLOCK_H
#define MODEL_BASICCLOCK_H

#include <vector>

#include "model/IClock.h"
#include "model/IClockSubscriber.h"

namespace model {

class IClockSubscriber;

class BasicClock : public IClock {
public:
  BasicClock() {}

  void addSubscriber(IClockSubscriber *sub) override {
    listeners.push_back(sub);
  }

  void advance() override {
    posEdge();
    negEdge();

    for (const auto &x : listeners)
      x->onAdvance();
  }

  void posEdge() override {
    for (const auto &x : listeners)
      x->onPosEdge();
  }

  void negEdge() override {
    for (const auto &x : listeners)
      x->onNegEdge();
  }

private:
  std::vector<IClockSubscriber *> listeners;
};

} // namespace model

#endif /* end of include guard: MODEL_BASICCLOCK_H */
