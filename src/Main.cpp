#include <iostream>
#include "WaterWaveOptimization.hpp"
#include "SphereFitnessFunction.hpp"

#define H_MAX 6

int main () {
  std::cout << "Water Wave Optimization!" << std::endl;

  SphereFitnessFunction sphereFitnessFunction = SphereFitnessFunction();
  WaterWaveOptimization wwo = WaterWaveOptimization(H_MAX, &sphereFitnessFunction);

  return 0;
}
