# Kruskal-Wallis H-test
from scipy.stats import kruskal, wilcoxon
import numpy as np

def run_test ():
	wwo_results = np.loadtxt(open("./python/results_51.csv", "rb"), delimiter=",")
	pso_results = np.loadtxt(open("./pso_results_51.csv", "rb"), delimiter=",")

	print(wwo_results)
	print(pso_results)

	for i in range(len(wwo_results) // 51):
		d = 52 * i
		wwo_result = wwo_results[d:d+52, 1]
		pso_result = pso_results[d:d+52, 1]

		stat, p = kruskal(wwo_result, pso_result)
		print('%.3f,%.10f' % (stat, p))

run_test()

# compare samples
# stat, p = kruskal(data1, data2, data3, data4, data5)
# print('Statistics=%.3f, p=%.10f' % (stat, p))
# # interpret
# alpha = 0.05
# if p > alpha:
# 	print('Same distributions (fail to reject H0)')
# else:
# 	print('Different distributions (reject H0)')

# print('wilcoxon: NB')
# stat, p = wilcoxon(data1, data2)
# print('Statistics=%.3f, p=%.5f' % (stat, p))

# print('wilcoxon: CART')
# stat, p = wilcoxon(data1, data3)
# print('Statistics=%.3f, p=%.5f' % (stat, p))

# print('wilcoxon: RL')
# stat, p = wilcoxon(data1, data4)
# print('Statistics=%.3f, p=%.5f' % (stat, p))

# print('wilcoxon: RF')
# stat, p = wilcoxon(data1, data5)
# print('Statistics=%.3f, p=%.5f' % (stat, p))