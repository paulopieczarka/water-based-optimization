#pragma once

#include <iostream>
#include "Wave.hpp"

class FitnessFunction {
  public:
    FitnessFunction () {};
    virtual double calc (Wave wave) = 0;
};
