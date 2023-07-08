// SPDX-FileCopyrightText: 2023 Serdar Sayın <https://serdarsayin.com>
//
// SPDX-License-Identifier: Apache-2.0

#ifndef MODEL_IPERFSTATS_H
#define MODEL_IPERFSTATS_H

namespace model {

class IPerfStats {
public:
  virtual ~IPerfStats() = default;
  virtual int getRetiredInstructions() const = 0;
  virtual int getTotalCycles() const = 0;
};

} // namespace model

#endif /* end of include guard: MODEL_IPERFSTATS_H */
