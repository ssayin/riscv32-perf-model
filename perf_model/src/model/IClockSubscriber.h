// SPDX-FileCopyrightText: 2023 Serdar Sayın <https://serdarsayin.com>
//
// SPDX-License-Identifier: Apache-2.0

#ifndef MODEL_ICLOCKSUBSCRIBER_H
#define MODEL_ICLOCKSUBSCRIBER_H

namespace model {

struct IClockSubscriber {
  virtual void onPosEdge() = 0;
  virtual void onNegEdge() = 0;
  virtual void onAdvance() = 0;
};

} // namespace model

#endif /* end of include guard: MODEL_ICLOCKSUBSCRIBER_H */
