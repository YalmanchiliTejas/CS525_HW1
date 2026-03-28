import matplotlib.pyplot as plt



p = [1,2,3,4,5,6,7,8]



n = [512, 1024, 1536, 2048, 2560, 3072, 3584, 4096]

times_baseline_1d = [0.004303, 0.017187, 0.021832, 0.053881, 0.084271, 0.084121, 0.143670, 0.185136]
times_baseline_2d = [0.005066, 0.019799, 0.024334, 0.062296, 0.076325, 0.101715, 0.129929, 0.311061]

times_reg_1d = [0.004377, 0.044081, 0.013135, 0.017613, 0.021789, 0.026293, 0.030616, 0.037000]
times_reg_2d = [0.005007, 0.056643, 0.016052, 0.020567, 0.025514, 0.030601, 0.035885, 0.040452]

speedup_1d = [baseline/ reg for baseline, reg in zip(times_baseline_1d, times_reg_1d)]
speedup_2d = [baseline/reg for baseline, reg in zip(times_baseline_2d, times_reg_2d)]

efficiency_1d = [s/p for s, p in zip(speedup_1d, p)]

efficiency_2d = [s/p for s, p in zip(speedup_2d, p)]

print("1D:")

for proc, size , s , e in zip(p, n, speedup_1d, efficiency_1d):
    print(f"p={proc}, n={size}, speedup={s}, effieciency={e}")

print("2D:")

for proc, size , s , e in zip(p, n, speedup_2d, efficiency_2d):
    print(f"p={proc}, n={size}, speedup={s}, effieciency={e}")


plt.figure(figsize=(8, 5))
plt.plot(p, speedup_1d, marker='o', label='1D Partitioning')
plt.plot(p, speedup_2d, marker='s', label='2D Partitioning')
plt.xlabel('Number of Processors (p)')
plt.ylabel('Speedup')
plt.title('Speedup vs Number of Processors for n = 512p')
plt.xticks(p)
plt.grid(True)
plt.legend()
plt.tight_layout()
plt.savefig("q4_3_speedup_plot.png", dpi=300, bbox_inches="tight")
plt.show()


plt.figure(figsize=(8, 5))
plt.plot(p, efficiency_1d, marker='o', label='1D Partitioning')
plt.plot(p, efficiency_2d, marker='s', label='2D Partitioning')
plt.xlabel('Number of Processors (p)')
plt.ylabel('Efficiency')
plt.title('Efficiency vs Number of Processors for n = 512p')
plt.xticks(p)
plt.grid(True)
plt.legend()
plt.tight_layout()
plt.savefig("q4_3_efficiency_plot.png", dpi=300, bbox_inches="tight")
plt.show()