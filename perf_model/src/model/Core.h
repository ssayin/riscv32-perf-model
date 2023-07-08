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
#include "IClock.h"
#include "ICore.h"
#include "model/BasicClockSubscriber.h"

namespace model {

template <class T> class Input {
public:
  void Write(T value) { t = value; };
  T Read() { return t; }

private:
  T t;
};

template <class T> class Output {
public:
  void Write(T value) { t = value; };
  T Read() { return t; }

private:
  T t;
};

struct MemoryRequest {
  enum Type : bool { READ, WRITE };
  uint32_t AddrBase;
  uint32_t NumBytes;
  Type ReqType;
  std::vector<uint8_t> Bytes;
};

struct FetchResponse {
  uint32_t AddrBase;
  uint32_t NumBytes;
  uint16_t Tag;
  std::vector<uint8_t> Bytes;
};

class FetchUnit : public BasicClockSubscriber {
public:
  FetchUnit(std::shared_ptr<BasicClock> clock)
      : BasicClockSubscriber(clock.get()), clk{clock} {}

  void onPosEdge() override;
  void onNegEdge() override;
  void onAdvance() override;

  Input<FetchResponse> PortFetchResponse;
  Output<MemoryRequest> PortMemoryRequest;

  void processResponses();
  void dispatchRequests();

private:
  std::shared_ptr<BasicClock> clk;
};

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
