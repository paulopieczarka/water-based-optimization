#pragma once

class SphereFunction : public Function {
  public:
    SphereFunction() {};

    double calcFitness (double *arr, int length) {
      double sum = 0;
      for (int i = 0; i < length; i++) {
        sum += arr[i] * arr[i];
      }

      return this->globalMinima() - sum;
    }

    std::tuple<double, double> domain () {
      return {-5.12, 5.12};
    }

    double globalMinima () {
      return 0.0;
    }
};
