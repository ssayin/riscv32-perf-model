// SPDX-FileCopyrightText: 2023 Serdar Sayın <https://serdarsayin.com>
//
// SPDX-License-Identifier: Apache-2.0

#ifndef MODEL_ICORE_H
#define MODEL_ICORE_H

#include "PerfStats.h"

namespace model {

class ICore {
public:
  virtual void advance() = 0;
  virtual const PerfStats &getStats() const = 0;
};

} // namespace model

#endif /* end of include guard: MODEL_ICORE_H */
