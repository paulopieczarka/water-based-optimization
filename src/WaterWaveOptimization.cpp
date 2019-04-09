#include "WaterWaveOptimization.hpp"
#include "Wave.hpp"

WaterWaveOptimization::WaterWaveOptimization (const int H_MAX) :
  H_MAX(H_MAX), population(5), features(10) {
  srand(time(NULL));
  this->waves = (Wave*)(malloc(this->population * sizeof(Wave)));
  this->generatePopulation();
}

WaterWaveOptimization::~WaterWaveOptimization () {

}

void WaterWaveOptimization::generatePopulation () {
  int i = 0;
  for (; i < this->population; i++) {
    this->waves[i] = Wave(H_MAX, this->features);
    this->waves[i].print();
  }
}
