#pragma once

#include <iostream>
#include <time.h>
#include <cmath>
#include <limits>
#include "Wave.hpp"
#include "Function.hpp"
#include "Random.hpp"

class WaterWaveOptimization {
  public:
    WaterWaveOptimization(const int H_MAX, Function *function);
    ~WaterWaveOptimization();

    void run(int maxIterations);

  private:
    const int H_MAX;
    const int K_MIN;
    const double ALPHA;
    const double BETA;

    int dimensions;
    int population;
    int maxBreakingDirections;

    Wave *waves;
    Wave *bestWave;
    Wave *worstWave;

    Function *function;

    void generatePopulation();
    void calcPopulationFitness();
    void updateWavelengths();

    void propagate(Wave *wave);
    void refract(Wave *newWave, Wave *wave);
    void breakWave(Wave *newWave, Wave *wave);
};
