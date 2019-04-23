#pragma once

#include <iostream>
#include <tuple>
#include "math.h"
#include "Wave.hpp"

#define _USE_MATH_DEFINES

class Function {
  public:
    Function () {};

    virtual double calcFitness (double *arr, int length) = 0;
    virtual std::tuple<double, double> domain () = 0;
    virtual double globalMinima () = 0;
    virtual bool isStopCriterionSatisfied (double fitness) = 0;

    bool doubleEquals(double left, double right, double epsilon) {
      return (fabs(left - right) < epsilon);
    }
};
