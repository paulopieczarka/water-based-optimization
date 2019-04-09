#include <iostream>
#include "WaterWaveOptimization.hpp"

#define H_MAX 6

int main () {
  std::cout << "Water Wave Optimization!" << std::endl;

  WaterWaveOptimization wwo = WaterWaveOptimization(H_MAX);

  return 0;
}
