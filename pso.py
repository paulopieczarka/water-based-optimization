from cec2019comp100digit import cec2019comp100digit
from joblib import Parallel, delayed
from pyswarm import pso
import multiprocessing
import numpy as np

def evalFunc (x, id, dimension):
  bench = cec2019comp100digit
  bench.init(id, dimension)

  return bench.eval(x)
  bench.end()

dimension = 10
lowers = np.full(dimension, -100.0)
uppers = np.full(dimension, 100.0)

def SaveReport (file):
  results = np.loadtxt(open(file, "rb"), delimiter=",")
  stats = []
  for i in  range(len(results) // 51):
    d = 52 * i
    result = results[d:d+52]
    stat = [
      np.min(result[:,1]),
      np.max(result[:,1]),
      np.average(result[:,1]),
      np.std(result[:,1])
    ]
    stats.append(stat)

  print(stats)
  np.savetxt("pdo_report_51.csv", stats, delimiter=",")

num_cores = multiprocessing.cpu_count()
def RunValidation (func, id=1, runs=52):
  print("Running ", runs, " times function (", id, ")")
  results = Parallel(n_jobs=num_cores)(delayed(pso)(func, lowers, uppers, maxiter=150000) for i in range(runs))
  return [ [id, fopt] for xopt, fopt in results ]

results = []
for i in range(1, 11):
  func = lambda x : evalFunc(x, i, dimension)
  results.extend(RunValidation(func, i))
  np.savetxt("pso_results_51.csv", results, delimiter=",")

SaveReport("pso_results_51.csv")
