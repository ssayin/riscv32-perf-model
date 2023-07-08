#include "Core.h"

void model::Core::onPosEdge() {}

void model::Core::onNegEdge() {}

void model::Core::onAdvance() { stats.Cycles++; }

const model::PerfStats &model::Core::getStats() const { return stats; }

void model::FetchUnit::onPosEdge() {
  processResponses();
  dispatchRequests();
}

void model::FetchUnit::onNegEdge() {}

void model::FetchUnit::onAdvance() {}

void model::FetchUnit::processResponses() {}

void model::FetchUnit::dispatchRequests() {}
