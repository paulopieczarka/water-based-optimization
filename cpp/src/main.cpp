#include <iostream>
#include "wwo.hpp"
#include "cec19_func.hpp"

double sphere_func (double arr[], int length) {
  double sum = 0;
  for (int i = 0; i < length; i++) {
    sum += std::pow(arr[i], 2);
  }

  return sum;
}

double ackley_func (double arr[], int length) {
  double n = length;
  double ninverse = 1 / n;

  double sum1 = 0;
  for (int i = 0; i < length; i++) {
    sum1 += std::pow(arr[i], 2);
  }

  double sum2 = 0;
  for (int i = 0; i < length; i++) {
    sum2 += std::cos(2 * M_PI * arr[i]);
  }

  return 20 + std::exp(1) - (20 * std::exp(-0.2 * std::sqrt(ninverse * sum1))) - std::exp(ninverse * sum2);
}

double griewank_func (double arr[], int length) {
  double sum = 0;
  double prod = 1;

  for (int i=0; i < length; i++) {
    sum += std::pow(arr[i], 2) / 4000;
    prod *= std::cos(arr[i] / std::sqrt(i+1));
  }

  return sum - prod + 1;
}

int main () {
  Cec19 cec19 = Cec19();
  WWO wwo = WWO();

  // std::cout << "-> start: Sphere" << std::endl;
  // wwo.exec(5, 10, 1, -100, 100);

  // std::cout << "-> start: Ackley" << std::endl;
  // wwo.exec(5, 10, 2, -100, 100);

  // std::cout << "-> start: Griewank" << std::endl;
  wwo.exec(5, 10, 8, -100, 100);

  // for (int i=1; i <= 10; i++) {
  //   wwo.exec(5, 10, i, -100, 100);
  // }

  return 0;
}
