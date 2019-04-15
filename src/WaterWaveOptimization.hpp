#pragma once

#include <iostream>
#include <time.h>
#include "Wave.hpp"
#include "Function.hpp"
#include "Random.hpp"

#define ALPHA = 1.01;
#define BETA = 0.01;

class WaterWaveOptimization {
  public:
    WaterWaveOptimization(const int H_MAX, Function *function);
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

    Wave *bestWave;

    Wave *waves;
    Function *function;
};
