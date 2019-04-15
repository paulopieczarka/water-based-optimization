#include "Wave.hpp"

Wave::Wave (const int H_MAX, int size, Function *function) :
  lenght(LAMBDA), fitness(0.0f), size(size), height(H_MAX), function(function) {
  this->values = (double*)(malloc(size * sizeof(double)));

  int i = 0;
  for (; i < this->size; i++) {
    this->values[i] = Random::BetweenDouble(
      std::get<0>(function->domain()),
      std::get<1>(function->domain())
    );
  }
}

Wave::~Wave () {

}

void Wave::print () {
  std::cout << "Wave [";

  int i = 0;
  for (; i < this->size; i++) {
    std::cout << this->values[i];

    if (i < this->size - 1) {
      std::cout << ", ";
    }
  }

  std::cout << "]";

  std::cout << " " << "{h:" << this->height;
  std::cout << ", l:" << this->lenght;
  std::cout << ", f:" << this->fitness << "}" << std::endl;
}
