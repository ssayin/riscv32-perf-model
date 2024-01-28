#include "FetchUnit.h"

void model::FetchUnit::onPosEdge() {
  processResponses();
  dispatchRequests();
}

void model::FetchUnit::onNegEdge() {}

void model::FetchUnit::onAdvance() {}

void model::FetchUnit::processResponses() {}

void model::FetchUnit::dispatchRequests() {}
