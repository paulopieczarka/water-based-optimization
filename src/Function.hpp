#pragma once

#include <iostream>
#include <tuple>
#include "Wave.hpp"

class Function {
  public:
    Function () {};

    virtual double calcFitness (double *arr, int length) = 0;
    virtual std::tuple<double, double> domain () = 0;
    virtual double globalMinima () = 0;
};
