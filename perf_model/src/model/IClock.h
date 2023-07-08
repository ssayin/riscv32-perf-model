// SPDX-FileCopyrightText: 2023 Serdar Sayın <https://serdarsayin.com>
//
// SPDX-License-Identifier: Apache-2.0

#ifndef MODEL_ICLOCK_H
#define MODEL_ICLOCK_H

#include "IPerfStats.h"

namespace model {

struct IClockSubscriber;
struct IClock {
  IClock() = default;
  ~IClock() = default;
  virtual void addSubscriber(IClockSubscriber *sub) = 0;
  virtual void advance() = 0;
  virtual void posEdge() = 0;
  virtual void negEdge() = 0;
};

} // namespace model

#endif /* end of include guard: MODEL_ICLOCK_H */
