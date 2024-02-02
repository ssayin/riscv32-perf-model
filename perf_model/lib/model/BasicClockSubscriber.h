// SPDX-FileCopyrightText: 2023 Serdar Sayın <https://serdarsayin.com>
//
// SPDX-License-Identifier: Apache-2.0

#ifndef MODEL_BASICCLOCKSUBSCRIBER_H
#define MODEL_BASICCLOCKSUBSCRIBER_H

#include "BasicClock.h"
#include "IClockSubscriber.h"

namespace model {

struct BasicClockSubscriber : public IClockSubscriber {
  BasicClockSubscriber(BasicClock *clk) { clk->addSubscriber(this); }
};

} // namespace model

#endif /* end of include guard: MODEL_BASICCLOCKSUBSCRIBER_H */
