#ifndef MODEL_ICLOCK_H
#define MODEL_ICLOCK_H

#include "IPerfStats.h"

namespace model {

class IClock {
public:
  IClock() = default;
  ~IClock() = default;
  virtual unsigned long getCurrentCycle() const = 0;
  virtual void advance() = 0;
};

} // namespace model

#endif /* end of include guard: MODEL_ICLOCK_H */
