#ifndef MODEL_FETCHUNIT_H
#define MODEL_FETCHUNIT_H

#include <memory>

#include "BasicClock.h"
#include "BasicClockSubscriber.h"
#include "MemoryRequest.h"

struct FetchResponse {
  uint32_t AddrBase;
  uint32_t NumBytes;
  uint16_t Tag;
  std::vector<uint8_t> Bytes;
};

namespace model {

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

}; // namespace model

#endif /* end of include guard: MODEL_FETCHUNIT_H */
