from cec2019comp100digit import cec2019comp100digit
from joblib import Parallel, delayed
import multiprocessing
import numpy as np
import random
import sys
import math
import time

def SphereFunc (x):
  return np.sum(np.power(x, 2))

def AckleyFunc (x, a=20, b=0.2, c=2*np.pi):
    d = len(x) # dimension of input vector x
    sum_sq_term = -a * np.exp(-b * np.sqrt(sum(x*x) / d))
    cos_term = -np.exp(sum(np.cos(c*x) / d))
    return a + np.exp(1) + sum_sq_term + cos_term

def evalFunc (x, id, dimension):
  bench = cec2019comp100digit
  bench.init(id, dimension)

  return bench.eval(x)
  bench.end()

def WWO (size, id, dimension, nfes = None, lowers = None, uppers = None):
  # constant
  if (lowers is None):
    lowers = np.full(dimension, -100.0)
  if (uppers is None):
    uppers = np.full(dimension, 100.0)
  if (nfes is None):
    nfes = dimension * 10000

  # parameter setting
  hMax = 12
  alpha = 1.001
  betaMax = 0.25
  betaMin = 0.001
  beta = betaMax
  kmax = min(12, dimension // 2)

  # initialiation
  x = Init(size, id, dimension, lowers, uppers)
  [minIndex, maxIndex] = MinIndexMaxIndex(x)
  optValue = x[minIndex][dimension]
  optVector = x[minIndex][:dimension]
  nfe = 0

  # print("-> optValue =", optValue)

  stTime = time.time()
  its = 0

  # iterative improvement
  u = np.zeros(dimension)
  while (nfe < nfes):
    its += 1
    for i in range(0, size):
      for d in range(0, dimension):
        u[d] = x[i][d] + (random.uniform(-1.0, 1.0) * x[i][dimension + 2] * (uppers[d] - lowers[d]))
        if (u[d] < lowers[d] or u[d] > uppers[d]):
          u[d] = lowers[d] + random.random() * (uppers[d] - lowers[d])
      newValue = evalFunc(u, id, dimension)
      nfe = nfe + 1
      if (newValue < x[i][dimension]):
        x[i][:dimension] = u.copy()
        x[i][dimension] = newValue
        x[i][dimension + 1] = hMax
        # Breaking
        if (newValue < optValue and i != minIndex):
          optValue = newValue
          k = kmax
          temp = np.random.permutation(dimension)[k:]
          for l in range(0, k):
            tempX = u.copy()
            d = temp[l]
            linearX = np.reshape(x, np.size(x), order='F').copy()
            tempX[d] = linearX[d] + random.normalvariate(0, 1) * beta * (uppers[d] - lowers[d])
            if (tempX[d] < lowers[d] or tempX[d] > uppers[d]):
              tempX[d] = lowers[d] + random.random() * (uppers[d] - lowers[d])
            newValueTemp = evalFunc(tempX, id, dimension)
            nfe = nfe + 1
            if (newValueTemp < newValue):
              linearX[d] = tempX[d]
              x = np.reshape(linearX, np.shape(x), order='F').copy()
              x[i][dimension + 2] = x[i][dimension + 2] * newValueTemp / x[i][dimension]
              x[i][dimension] = newValueTemp
              newValue = newValueTemp
              if (newValueTemp < optValue):
                optValue = newValueTemp
                optVector = x[i][:dimension]
      else:
        # decrease wave height
        x[i][dimension + 1] -= 1
        # Refraction
        if (x[i][dimension + 1] == 0):
          for d in range(0, dimension):
            u[d] = random.normalvariate((optVector[d]+x[i][d])/2, abs(optVector[d] - x[i][d])/2)
            if (u[d] < lowers[d] or u[d] > uppers[d]):
              u[d] = lowers[d] + random.random() * (uppers[d] - lowers[d])
          oldValue = x[i][dimension]
          x[i][:dimension] = u.copy()
          x[i][dimension] = evalFunc(u, id, dimension)
          nfe = nfe + 1
          x[i][dimension + 1] = hMax
          x[i][dimension + 2] = x[i][dimension + 2] * x[i][dimension] / oldValue

    [minIndex, maxIndex] = MinIndexMaxIndex(x)
    worstValue = x[maxIndex][dimension]
    optValue = x[minIndex][dimension]
    optVector = x[minIndex]
    den = (worstValue - optValue + sys.float_info.epsilon)
    for i in range(0, size):
      x[i][dimension + 2] = x[i][dimension + 2] * pow(alpha, -(worstValue-x[i][dimension]+sys.float_info.epsilon) / den)
    beta = betaMax - (betaMax - betaMin) * nfe / nfes

  [minIndex, maxIndex] = MinIndexMaxIndex(x)
  optValue = x[minIndex][dimension]
  optVector = x[minIndex]

  execTime = time.time()-stTime
  return [id, optValue, execTime, its]

def Init(size, id, dimension, lowers, uppers):
  hmax = 12
  wlambda = 0.5
  x = np.zeros((size, dimension + 3))

  for i in range(0, size):
    for j in range(0, dimension):
      x[i][j] = lowers[j] + random.random() * (uppers[j] - lowers[j])
    x[i][dimension] = evalFunc(x[i], id, dimension)
    x[i][dimension + 1] = hmax
    x[i][dimension + 2] = wlambda

  return x

# Get the indicies of best and worst solutions of the population
def MinIndexMaxIndex (x):
  [m, n] = np.shape(x)
  minIndex = 0
  maxIndex = 0
  bestValue = x[0][n - 3]
  worstValue = bestValue

  for i in range(1, m):
    if (x[i][n - 3] < bestValue):
      minIndex = i
      bestValue = x[i][n - 3]
    elif (x[i][n - 3] > worstValue):
      maxIndex = i
      worstValue = x[i][n - 3]

  return [minIndex, maxIndex]

num_cores = multiprocessing.cpu_count()
def RunValidation (id=1, runs=51):
  print("Running ", runs, " times function (", id, ")")
  results = results = Parallel(n_jobs=num_cores)(delayed(WWO)(5, id, 10, nfes=150000) for i in range(runs))
  results.append(WWO(5, id, 10, nfes=150000))
  # for i in range(1, runs+1):
  #   print("Running Function(", id, ")", i, "of", runs, "times...")
  #   results.append(WWO(5, id, 10, nfes=150000))

  print(results)
  return results

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
  np.savetxt("report.csv", stats, delimiter=",")

print('WWO 5 population, 10 dimensions in ', num_cores, 'cores')
report = []
for i in range(1, 11):
  report.extend(RunValidation(i))
  np.savetxt("results_51.csv", report, delimiter=",")

SaveReport("results_51.csv")

# RunCECBenchmark()

# WWO(5, 1, 10)
# print('WWO 5 population, 30 dimensions')
# WWO(5, 1, 30)
