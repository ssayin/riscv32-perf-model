#include "model/Core.h"
#include "model/FetchUnit.h"

void model::Core::onPosEdge() {}

void model::Core::onNegEdge() {}

void model::Core::onAdvance() { stats.Cycles++; }

const model::PerfStats &model::Core::getStats() const { return stats; }
