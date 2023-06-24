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
