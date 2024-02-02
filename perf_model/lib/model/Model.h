#ifndef MODEL_MODEL_H
#define MODEL_MODEL_H

#include "BasicClock.h"
#include "BasicClockSubscriber.h"
#include "Core.h"
#include <memory>

namespace model {

class Model : public BasicClockSubscriber {
public:
  Model(std::string id, std::shared_ptr<BasicClock> clock)
      : BasicClockSubscriber(clock.get()), id{id}, clk{{clock}},
        core{{"core0", clock}} {}

  void onPosEdge() override;
  void onNegEdge() override;
  void onAdvance() override;

  std::vector<Core> core;

private:
  std::vector<std::shared_ptr<BasicClock>> clk;
  std::string id;
};

} // namespace model

#endif /* end of include guard: MODEL_MODEL_H */
