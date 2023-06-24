#ifndef MODEL_ATOMICCLOCK_H
#define MODEL_ATOMICCLOCK_H

#include <atomic>

#include "IClock.h"

namespace model {

class AtomicClock : public IClock {
public:
  AtomicClock() : CurCycle{0} {}
  unsigned long getCurrentCycle() const override { return CurCycle.load(); }
  void advance() override { CurCycle++; }

private:
  std::atomic<unsigned long> CurCycle;
};

} // namespace model

#endif /* end of include guard: MODEL_ATOMICCLOCK_H */
