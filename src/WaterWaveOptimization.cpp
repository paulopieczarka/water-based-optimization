#include "WaterWaveOptimization.hpp"

WaterWaveOptimization::WaterWaveOptimization (const int H_MAX, Function *function) :
  H_MAX(H_MAX),
  K_MIN(12),
  ALPHA(1.01),
  BETA(0.001),
  population(5),
  dimensions(2),
  function(function),
  bestWave(NULL)
{
  srand(time(NULL));
  this->waves = (Wave*)(malloc(this->population * sizeof(Wave)));
  this->generatePopulation();
  this->maxBreakingDirections = std::min(K_MIN, this->dimensions / 2);
}

WaterWaveOptimization::~WaterWaveOptimization () {
  // De-allocate stuff
}

void WaterWaveOptimization::run (int maxIterations) {
  this->calcPopulationFitness();

  int i = 0;
  for (; i < this->population; i++) {
    if(this->bestWave == NULL || this->waves[i].fitness < this->bestWave->fitness) {
      this->bestWave = this->waves + i;
    }
  }

  for (int j=0; j < maxIterations; j++) {
    for (i = 0; i < this->population; i++) {
      this->propagate(this->waves + i);
    }

    this->updateWavelengths();

    std::cout << "It(" << (j + 1) << ") Best";
    this->bestWave->print();
    // std::cout << std::endl;

    if (function->isStopCriterionSatisfied(bestWave->fitness)) {
      break;
    }
  }
}

void WaterWaveOptimization::generatePopulation () {
  for (int i = 0; i < this->population; i++) {
    this->waves[i] = Wave(H_MAX, this->dimensions, this->function);
  }
}

void WaterWaveOptimization::calcPopulationFitness () {
  for (int i = 0; i < this->population; i++) {
    Wave *wave = &(this->waves[i]);
    double f = 0;
    f = function->calcFitness(wave->values, wave->size);
    wave->fitness = f;
    wave->print();
  }
}

void WaterWaveOptimization::propagate (Wave *wave) {
  Wave newWave = Wave(*wave);

  double *x = wave->values;
  double *y = newWave.values;

  int d = 0;
  for (; d < wave->size; d++) {
    y[d] = x[d] + Random::BetweenInt(-1, 1) * wave->lenght * (d + 1);
  }

  newWave.fitness = function->calcFitness(newWave.values, wave->size);

  if (newWave.fitness < wave->fitness) {
    if (newWave.fitness < bestWave->fitness) {
      this->breakWave(&newWave, wave);
      *bestWave = Wave(newWave);
    }

    *wave = Wave(newWave);
  } else {
    wave->height -= 1;
    if (wave->height == 0) {
      this->refract(&newWave, wave);
      *wave = Wave(newWave);
    }
  }
}

void WaterWaveOptimization::refract (Wave *newWave, Wave *wave) {
  // Refract eq. 8
  double midPosX;
  double midPosY;
  double *x = wave->values;
  double *y = newWave->values;

  for (int d = 0; d < wave->size; d++) {
    midPosX = (bestWave->values[d] + x[d]) / 2;
    midPosY = std::abs((bestWave->values[d] - x[d])) / 2;
    y[d] = Random::GaussianDouble(midPosX, midPosY);
  }

  // Reset height to hMax
  newWave->height = H_MAX;

  // Set have length with eq. 9
  // std::cout << "\n\n" << newWave->fitness << " -> ";
  newWave->fitness = function->calcFitness(newWave->values, wave->size);
  // std::cout << newWave->fitness << "\n\n" << std::endl;
  newWave->lenght = wave->lenght * (wave->fitness / newWave->fitness);
}

void WaterWaveOptimization::breakWave (Wave *newWave, Wave *wave) {
  double *x = wave->values;
  Wave solitaryWave = Wave(*newWave);
  Wave currentBestWave = Wave(*newWave);

  int k = Random::BetweenInt(1, this->maxBreakingDirections);
  for (int i = 0; i < k; i++) {
    solitaryWave = Wave(*newWave);

    for (int d = 0; d < wave->size; d++) {
      solitaryWave.values[d] = x[d] + Random::GaussianDouble(0, 1) * this->BETA * (d + 1);
    }

    solitaryWave.fitness = function->calcFitness(solitaryWave.values, solitaryWave.size);
    if (solitaryWave.fitness < currentBestWave.fitness) {
      currentBestWave = Wave(solitaryWave);
    }
  }

  *newWave = Wave(currentBestWave);
}

void WaterWaveOptimization::updateWavelengths () {
  int i = 0;
  Wave *wave;
  double minFitness = std::numeric_limits<double>::max();

  for (; i < this->population; i++) {
    wave = this->waves + i;
    if (wave->fitness > minFitness) {
      minFitness = wave->fitness;
    }
  }


  for (i = 0; i < this->population; i++) {
    wave = this->waves + i;
    double expo =
      (wave->fitness - minFitness + std::numeric_limits<double>::epsilon()) /
      (bestWave->fitness - minFitness + std::numeric_limits<double>::epsilon())
    ;
    wave->lenght = wave->lenght * pow(this->ALPHA, expo * (-1));
  }
}
