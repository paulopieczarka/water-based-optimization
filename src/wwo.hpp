#include <iostream>
#include <random>

class WWO {
  public:
    void exec(int size, int dimension, double (*func)(double[], int), double lower, double upper);

    double** init(int size, int dimension, double *lowers, double *uppers, double (*func)(double[], int));

  private:
    int* minIndexMaxIndex(double **x, int size, int dimension);
    int* randperm(int n, int k);

    double nrand();
    double normrnd(double mu, double sigma);
    double* n_array(int d, int x);
    double** n_matrix(int m, int n, double x);
};
