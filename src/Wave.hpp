#pragma once

#include <iostream>
#include "Random.hpp"
#include "Function.hpp"

#define LAMBDA 0.5f

class Wave {
  public:
    float lenght;
    float fitness;

    int size;
    int height;

    double *values;

    Wave(const int H_MAX, int size, Function *function);
    ~Wave();

    void print();

  private:
    Function *function;
};
