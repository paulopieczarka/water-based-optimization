#include "wwo.hpp"

double* WWO::n_array (int d, int x) {
  double *arr = new double[d];
  for (int i=0; i < d; i++) {
    arr[i] = x;
  }

  return arr;
}

double** WWO::n_matrix (int m, int n, double x) {
  double **mat = new double*[m];
  for (int i=0; i < m; i++) {
    mat[i] = new double[n];
    for (int j=0; j < n; j++) {
      mat[i][j] = x;
    }
  }

  return mat;
}

double WWO::nrand () {
  return ((double) std::rand() / __INT_MAX__);
}

double** WWO::init (int size, int dimension, double *lowers, double *uppers, double (*func)(double[], int)) {
  const int hMax = 12;
  double lambda = 0.5;

  double **x = n_matrix(size, dimension + 2, 0.0);

  for (int i=0; i < size; i++) {
    for (int j=0; j < dimension; j++) {
      x[i][j] = lowers[j] + nrand() * (uppers[j] - lowers[j]);
    }

    x[i][dimension] = func(x[i], dimension);
    x[i][dimension + 1] = hMax;
    x[i][dimension + 2] = lambda;
  }

  return x;
}

// Get the indicies of best and worst solutions of the population
int* WWO::minIndexMaxIndex (double **x, int size, int dimension) {
  int minIndex = 0;
  int maxIndex = 0;

  double bestValue = x[0][dimension];
  double worstValue = bestValue;

  for (int i = 1; i < size; i++) {
    if (x[i][dimension] < bestValue) {
      minIndex = i;
      bestValue = x[i][dimension];
    } else if (x[i][dimension] > worstValue) {
      maxIndex = i;
      worstValue = x[i][dimension];
    }
  }

  return new int[2]{minIndex, maxIndex};
}

int* WWO::randperm (int n, int k) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> rand(1, k);

  int *result = new int[n];
  for (int i=0; i < n; i++) {
    result[i] = rand(gen);
  }

  return result;
}

double WWO::normrnd (double mu, double sigma) {
  std::default_random_engine generator;
  std::normal_distribution<double> distribution(mu, sigma);

  return distribution(generator);
}

void WWO::exec (int size, int dimension, double (*func)(double[], int), double lower, double upper) {
  // std::cout << "-> start" << std::endl;

  // lower bounds
  double *lowers = n_array(dimension, lower);

  // upper bounds
  double *uppers = n_array(dimension, upper);

  // number of function evaluations
  double nfes = dimension * 100000;

  double epsilon = 0.0000001;

  // parameter setting
  int hMax = 12;
  double alpha = 1.001;
  double betaMax = 0.25;
  double betaMin = 0.001;
  double beta = betaMax;
  int kmax = std::min(12, dimension / 2);

  // initialization
  // std::cout << "-> init" << std::endl;

  double **x = init(size, dimension, lowers, uppers, func);

  int *indexs = minIndexMaxIndex(x, size, dimension);
  int minIndex = indexs[0];
  int maxIndex = indexs[1];

  double optValue = x[minIndex][dimension];
  double optVector[dimension];
  std::copy(x[minIndex], x[minIndex] + dimension, optVector);

  std::cout << "-> optValue=" << optValue << std::endl;

  int nfe = 0;

  // iterative improvement
  // std::cout << "-> run" << std::endl;
  double *u = n_array(dimension, 0.0);

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<double> unifrnd(-1, 1);

  while (nfe < nfes) {
    // std::cout << "-> nfe(" << nfe << ") " << std::endl;

    // Propagation
    for (int i=0; i < size; i++) {
      for (int d=0; d < dimension; d++) {
        u[d] = x[i][d] + unifrnd(gen) * x[i][dimension + 2] * (uppers[d] - lowers[d]);
        if (u[d] < lowers[d] || u[d] > uppers[d]) {
          u[d] = lowers[d] + nrand() * (uppers[d] - lowers[d]);
        }
      }

      double newValue = func(u, dimension);
      nfe += 1;

      if (newValue < x[i][dimension]) {
        std::copy(u, u + dimension, x[i]);
        x[i][dimension] = newValue;
        x[i][dimension + 1] = hMax;

        // Breaking
        if (newValue < optValue && i != minIndex) {
          optValue = newValue;
          int k = kmax;
          int *temp = randperm(dimension, k);

          for (int l=0; l < k; l++) {
            double tempX[dimension];
            std::copy(u, u + dimension, tempX);
            int d = temp[l];
            tempX[d] = x[i][d] * normrnd(0, 1) * beta * (uppers[d] - lowers[d]);
            if (tempX[d] < lowers[d] || tempX[d] > uppers[d]) {
              tempX[d] = lowers[d] + nrand() * (uppers[d] - lowers[d]);
            }

            double newValueTemp = func(tempX, dimension);
            nfe += 1;

            if (newValueTemp < newValue) {
              std::copy(tempX, tempX + dimension, x[i]);
              x[i][dimension + 2] *= newValueTemp / x[i][dimension];
              x[i][dimension] = newValueTemp;
              newValue = newValueTemp;

              if (newValueTemp < optValue) {
                optValue = newValueTemp;
                std::copy(tempX, tempX + dimension, optVector);
              }
            }
          }
        }
      } else {
        // decrease wave height
        x[i][dimension + 1] = x[i][dimension + 1] - 1;

        // Refraction
        if (x[i][dimension + 1] == 0) {
          for (int d=0; d < dimension; d++) {
            u[d] = normrnd((optVector[d] + x[i][d]) / 2, std::abs((optVector[d] - x[i][d]) / 2));
            if (u[d] < lowers[d] || u[d] > uppers[d]) {
              u[d] = lowers[d] + nrand() * (uppers[d] - lowers[d]);
            }
          }

          double oldValue = x[i][dimension];
          std::copy(u, u + dimension, x[i]);
          x[i][dimension] = func(u, dimension);
          nfe += 1;
          x[i][dimension + 1] = hMax;
          x[i][dimension + 2] *= x[i][dimension + 1] / oldValue;
        }
      }
    }

    // Update wavelenghts
    indexs = minIndexMaxIndex(x, size, dimension);
    minIndex = indexs[0];
    maxIndex = indexs[1];

    optValue = x[minIndex][dimension];
    std::copy(x[minIndex], x[minIndex] + dimension, optVector);

    double worstValue = x[maxIndex][dimension];
    double den = (worstValue - optValue + epsilon);
    for (int i=0; i < size; i++) {
      x[i][dimension + 2] *= std::pow(alpha, - (worstValue - x[i][dimension] + epsilon) / den);
    }

    beta = betaMax - (betaMax - betaMin) * nfe / nfes;
  }

  indexs = minIndexMaxIndex(x, size, dimension);
  minIndex = indexs[0];
  maxIndex = indexs[1];

  optValue = x[minIndex][dimension];
  std::copy(x[minIndex], x[minIndex] + dimension, optVector);

  std::cout << "-> optVector = [";
  for (int i=0; i < dimension; i++) {
    std::cout << optVector[i] << ((i == dimension-1) ? "" : ", ");
  }

  std::cout << "]" << std::endl;
  std::cout << "-> optValue = " << optValue << std::endl;
  std::cout << std::endl;
}
