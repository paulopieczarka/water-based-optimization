#include "WaterWaveOptimization.hpp"
#include "Wave.hpp"

WaterWaveOptimization::WaterWaveOptimization (const int H_MAX, FitnessFunction *fitnessFunction) :
  H_MAX(H_MAX), population(5), features(10), fitnessFunction(fitnessFunction) {
  srand(time(NULL));
  this->waves = (Wave*)(malloc(this->population * sizeof(Wave)));
  this->generatePopulation();
}

WaterWaveOptimization::~WaterWaveOptimization () {
  // De-allocate stuff
}

void WaterWaveOptimization::generatePopulation () {
  int i = 0;
  for (; i < this->population; i++) {
    this->waves[i] = Wave(H_MAX, this->features);
  }

  this->calcPopulationFitness();
}

void WaterWaveOptimization::calcPopulationFitness () {
  int i = 0;
  for (; i < this->population; i++) {
    double fitness = this->fitnessFunction->calc(this->waves[i]);
    this->waves[i].fitness = fitness;
    this->waves[i].print();
  }
}

void WaterWaveOptimization::propagate (Wave *wave) {

}

void WaterWaveOptimization::refract (Wave *wave) {

}

void WaterWaveOptimization::breakWave (Wave *wave) {

}
