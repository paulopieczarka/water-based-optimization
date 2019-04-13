#include "FitnessFunction.hpp"

class SphereFitnessFunction : public FitnessFunction {
  public:
    SphereFitnessFunction() {};

    double calc (Wave wave) {
      int sum = 0;

      int i = 0;
      for (; i < wave.size; i++) {
        sum += wave.values[i] * wave.values[i];
      }

      return sum;
    }
};
