#ifndef MODEL_CORE_H
#define MODEL_CORE_H

#include <memory>
#include <string>

#include "IClock.h"
#include "ICore.h"

namespace model {

class Core : public ICore {
public:
  Core(std::string id, std::shared_ptr<IClock> clock) : clk{clock} {}

  void advance() override {
    clk->advance();
    stats.Cycles++;
  };

  const PerfStats &getStats() const override { return stats; }

private:
  std::shared_ptr<IClock> clk;
  PerfStats stats;
};

} // namespace model

#endif /* end of include guard: MODEL_CORE_H */
