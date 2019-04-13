#pragma once

#include <iostream>
#include <time.h>
#include "Wave.hpp"
#include "FitnessFunction.hpp"

class WaterWaveOptimization {
  public:
    WaterWaveOptimization(const int H_MAX, FitnessFunction *fitnessFunction);
    ~WaterWaveOptimization();

    void generatePopulation();
    void calcPopulationFitness();

    void propagate(Wave *wave);
    void refract(Wave *wave);
    void breakWave(Wave *wave);

  private:
    const int H_MAX;

    int features;
    int population;

    Wave *waves;
    FitnessFunction *fitnessFunction;
};
