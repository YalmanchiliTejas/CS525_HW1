import matplotlib.pyplot as plt



p = [1,2,3,4,5,6,7,8]

times_1d =  [0.070906, 0.172216, 0.024057, 0.018561, 0.014297, 0.012591, 0.010532, 0.009138]

times_2d =  [0.044090, 0.101673, 0.027435, 0.020550, 0.017309, 0.014258, 0.012308, 0.010680]




serial_time_1d = times_1d[0]
serial_time_2d = times_2d[0]


speedup_1d = [serial_time_1d / times_1d[i] for i in range(len(times_1d))]
speedup_2d = [serial_time_2d / times_2d[i] for i in range(len(times_2d))]

efficiency_1d = [speedup/p for speedup, p in zip(speedup_1d, p)]

efficiency_2d = [speedup/p for speedup, p in zip(speedup_2d, p)]

speedup_1d_plot = speedup_1d[1:]
speedup_2d_plot = speedup_2d[1:]
efficiency_1d_plot = efficiency_1d[1:]
efficiency_2d_plot = efficiency_2d[1:]

p_plot = p[1:]
print("1d:")
for p, s, e in zip(p_plot,speedup_1d_plot,efficiency_1d_plot ):
    print(f"p={p}, speedup={s}, effieciency={e}")
print("2d:")
for p, s, e in zip(p_plot,speedup_2d_plot,efficiency_2d_plot ):
    print(f"p={p}, speedup={s}, effieciency={e}")

plt.figure(figsize=(8, 5))
plt.plot(p_plot, speedup_1d_plot, marker='o', label='1D Partitioning')
plt.plot(p_plot, speedup_2d_plot, marker='s', label='2D Partitioning')

plt.xlabel('Number of Processors (p)')
plt.ylabel('Speedup')
plt.title('Speedup vs Number of Processors')
plt.xticks(p_plot)
plt.grid(True)
plt.legend()
plt.tight_layout()
plt.show()
plt.savefig("q4_1_speedup_plot.png", dpi=300, bbox_inches="tight")


plt.figure(figsize=(8, 5))
plt.plot(p_plot, efficiency_1d_plot, marker='o', label='1D Partitioning')
plt.plot(p_plot, efficiency_2d_plot, marker='s', label='2D Partitioning')
plt.xlabel('Number of Processors (p)')
plt.ylabel('Efficiency')
plt.title('Efficiency vs Number of Processors')
plt.xticks(p_plot)
plt.grid(True)
plt.legend()
plt.tight_layout()
plt.show()

plt.savefig("q4_1_efficiency_plot.png", dpi=300, bbox_inches="tight")


