#include <iostream>
#include "WaterWaveOptimization.hpp"
#include "SphereFunction.hpp"
#include "AckleyFunction.hpp"

#define H_MAX 6

int main () {
  std::cout << "Water Wave Optimization!" << std::endl;

  // SphereFunction f = SphereFunction();
  AckleyFunction f = AckleyFunction();
  WaterWaveOptimization wwo = WaterWaveOptimization(H_MAX, &f);
  wwo.run(1000000);

  return 0;
}
