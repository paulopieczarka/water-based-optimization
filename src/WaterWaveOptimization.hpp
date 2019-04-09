#pragma once

#include <iostream>
#include <time.h>
#include "Wave.hpp"

class WaterWaveOptimization {
  public:
    WaterWaveOptimization(const int H_MAX);
    ~WaterWaveOptimization();

    void generatePopulation();

  private:
    const int H_MAX;

    int features;
    int population;
    Wave *waves;
};
