#pragma once

#include <iostream>
#include <random>
#include "time.h"

#define uniform() (rand()/(RAND_MAX + 1.0))

class Random {
  public:
    Random () {}

    static double Double(double n);
    static double BetweenDouble(double min, double max);
    static double GaussianDouble(double mean, double deviation);

    static int Int(int n);
    static int BetweenInt(int min, int max);

};
