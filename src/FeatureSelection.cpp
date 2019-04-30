#include <iostream>
#include <dlib/svm.h>
#include "DataSet.hpp"

using namespace std;
using namespace dlib;

int main()
{
  DataSet dataset = DataSet("../datasets/plat-sizes-small.data");
  dataset.read();

  std::cout << "-> [1] Opening dataset..";

  typedef matrix<double, 64, 1> sampleType;
  typedef radial_basis_kernel<sampleType> kernelType;

  std::vector<sampleType> samples;
  std::vector<double> labels;

  for (int i=0; i < dataset.instances(); i++) {
    sampleType sample;
    for (int j=0; j < dataset.features(); j++) {
      if (j == dataset.features() - 1) {
        labels.push_back(dataset.itemAsDouble(i, j));
      } else {
        sample(j) = dataset.itemAsDouble(i, j);
      }
    }

    samples.push_back(sample);
  }

  std::cout << " (samples=" << samples.size() << ")";
  std::cout << " Done!" << std::endl << "-> [2] Normalizing dataset..";

  vector_normalizer<sampleType> normalizer;
  normalizer.train(samples);

  for (unsigned long i = 0; i < samples.size(); ++i) {
    samples[i] = normalizer(samples[i]);
  }

  std::cout << " Done!" << std::endl << "-> [3] Randomizing samples..";

  randomize_samples(samples, labels);

  std::cout << " Done!" << std::endl << "-> [4] Training model..";

  svr_trainer<kernelType> trainer;
  trainer.set_kernel(kernelType(0.1));
  trainer.set_epsilon_insensitivity(0.001);
  trainer.set_c(10);

  decision_function<kernelType> df = trainer.train(samples, labels);

  std::cout << " Done!" << std::endl << "-> [5] Validating model: ";

  randomize_samples(samples, labels);
  matrix<double, 1, 4> results = cross_validate_regression_trainer(trainer, samples, labels, 5);

  std::cout << "Correlation: "<< results(0) << std::endl;

  std::cout << "-> End!" << std::endl;
}
