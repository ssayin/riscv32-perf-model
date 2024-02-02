// SPDX-FileCopyrightText: 2023 Serdar Sayın <https://serdarsayin.com>
//
// SPDX-License-Identifier: Apache-2.0

#ifndef MODEL_PERFSTATS_H
#define MODEL_PERFSTATS_H

#include "IPerfStats.h"

namespace model {

class PerfStats : public IPerfStats {
public:
  friend class Core;

  int getRetiredInstructions() const override { return InstrRetired; }
  int getTotalCycles() const override { return Cycles; }

  PerfStats &operator+=(const PerfStats &other) {
    InstrRetired += other.InstrRetired;
    Cycles += other.Cycles;
    return *this;
  }

private:
  int InstrRetired = 0;
  int Cycles = 0;
};

} // namespace model

#endif /* end of include guard: MODEL_PERFSTATS_H */
