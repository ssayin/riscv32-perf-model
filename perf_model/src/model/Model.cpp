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
void model::Model::playback(std::vector<VisibleState> &visible) {
  for (auto &it : visible) {
    std::cout << it.instr << std::endl;
  }
}
