#include "Model.h"

#include <iostream>

void model::Model::onNegEdge() {
  for (auto &x : core) {
    x.onNegEdge();
  }
}

void model::Model::onAdvance() {
  for (auto &x : core) {
    x.onAdvance();
  }
}

void model::Model::onPosEdge() {
  for (auto &x : core) {
    x.onPosEdge();
  }
}
