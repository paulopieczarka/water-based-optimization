#include <iostream>
#include "WaterWaveOptimization.hpp"
#include "SphereFunction.hpp"

#define H_MAX 6

int main () {
  std::cout << "Water Wave Optimization!" << std::endl;

  SphereFunction sphereFunction = SphereFunction();
  WaterWaveOptimization wwo = WaterWaveOptimization(H_MAX, &sphereFunction);

  return 0;
}
