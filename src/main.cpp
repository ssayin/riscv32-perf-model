#include <bitset>

#include "common/common.hpp"
#include "nlohmann/json.hpp"
#include "spdlog/spdlog.h"

int main(int argc, char **argv) {
  spdlog::info("Starting model");

  spdlog::info(std::bitset<32>{masks::sign_bit}.to_string());

  return 0;
}
