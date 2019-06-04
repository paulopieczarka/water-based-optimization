#include <iostream>
#include <random>
#include "cec19_func.hpp"

double* matrix2ContiguousArray(double** x, int size, int dimension);
double** contiguousArray2Matrix(double* x, int size, int dimension);

class WWO {
  public:
    WWO();

    void exec(int size, int dimension, int funcId, double lower, double upper);

    double** init(int size, int dimension, double *lowers, double *uppers, int funcId);

  private:
    Cec19 cec19;

    int* minIndexMaxIndex(double **x, int size, int dimension);
    int* randperm(int n, int k);

    double nrand();
    double normrnd(double mu, double sigma);
    double* n_array(int d, int x);
    double** n_matrix(int m, int n, double x);
};
