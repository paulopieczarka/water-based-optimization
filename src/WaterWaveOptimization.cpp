#include "WaterWaveOptimization.hpp"

WaterWaveOptimization::WaterWaveOptimization (const int H_MAX, Function *function) :
  H_MAX(H_MAX), population(5), features(10), function(function), bestWave(NULL) {
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
    this->waves[i] = Wave(H_MAX, this->features, this->function);
  }

  this->calcPopulationFitness();

  for (i = 0; i < this->population; i++) {
    this->propagate(this->waves + i);
  }
}

void WaterWaveOptimization::calcPopulationFitness () {
  int i = 0;
  for (; i < this->population; i++) {
    Wave *wave = &(this->waves[i]);
    double fitness = this->function->calcFitness(wave->values, wave->size);
    this->waves[i].fitness = fitness;

    if (bestWave == NULL || fitness > bestWave->fitness) {
      this->bestWave = wave;
    }

    this->waves[i].print();
  }
}

void WaterWaveOptimization::propagate (Wave *wave) {
  double *x = wave->values;
  double *y = (double*)malloc(wave->size * sizeof(double));
  std::copy(x, x + wave->size, y);

  int d = 0;
  for (; d < wave->size; d++) {
    y[d] = x[d] + Random::BetweenInt(-1, 1) * wave->lenght * wave->size;
  }

  double yFitness = this->function->calcFitness(y, wave->size);

  if (yFitness > wave->fitness) {
    std::copy(y, y + wave->size, x);
    wave->fitness = yFitness;

    if (yFitness > bestWave->fitness) {
      this->breakWave(wave);
      this->bestWave = wave;
      std::cout << "-> New Best ";
      wave->print();
    }
  }
}

void WaterWaveOptimization::refract (Wave *wave) {

}

void WaterWaveOptimization::breakWave (Wave *wave) {
  double *x = wave->values;

  int d = 0;
  for (; d < wave->size; d++) {
    x[d] = x[d] + Random::GaussianDouble(0, 1) * 0.01 * wave->size;
  }
}
