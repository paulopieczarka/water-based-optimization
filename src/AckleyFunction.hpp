#pragma once

#include "Function.hpp"

class AckleyFunction : public Function {
  public:
    AckleyFunction() {};

    double calcFitness (double *arr, int length) {
      double n = length;
      double ninverse = 1 / n;

      double sum1 = 0;
      for (int i = 0; i < length; i++) {
        sum1 += pow(arr[i], 2);
      }

      double sum2 = 0;
      for (int i = 0; i < length; i++) {
        sum2 += std::cos(2 * M_PI * arr[i]);
      }

      return 20 + exp(1) - (20 * exp(-0.2 * sqrt(ninverse * sum1))) - exp(ninverse * sum2);
    }

    std::tuple<double, double> domain () {
      return {-32, 32};
    }

    double globalMinima () {
      return 0.0;
    }

    bool isStopCriterionSatisfied (double fitness) {
      return fitness == this->globalMinima();
    }
};
