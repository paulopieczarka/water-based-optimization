#include "Random.hpp"

double Random::Double (double n) {
  return (uniform() * n);
}

double Random::BetweenDouble (double min, double max) {
  return min + (int)((double)(max - min + 1) * (rand() / (RAND_MAX + 1.0)));
}

double Random::GaussianDouble (double mean, double deviation) {
  std::default_random_engine generator;
  std::normal_distribution<double> distribution(mean, deviation);
  return distribution(generator);
}

int Random::Int (int n) {
  return (int)Random::Double(n);
}

int Random::BetweenInt (int min, int max) {
  return (int)Random::BetweenDouble(min, max);
}
