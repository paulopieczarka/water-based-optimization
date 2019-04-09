#pragma once

#include <iostream>

#define LAMBDA 0.5f

struct Wave {
  float lenght;
  float fitness;

  int size;
  int height;
  int *values;

  Wave (const int H_MAX, int size) :
    lenght(LAMBDA), fitness(0.0f), size(size), height(H_MAX) {
    this->values = (int*)(malloc(size * sizeof(int)));

    int i = 0;
    for (; i < this->size; i++) {
      this->values[i] = rand() % 2;
    }
  }

  void print () {
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
};
