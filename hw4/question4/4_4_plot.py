import matplotlib.pyplot as plt


W = [1,4,9,16,25,36,49,64]

p_1d = [1,2,3,4,5,6,7,8]
p_2d = [1,2,3,4,5,6,7,8]


serial_1d = [0.004351, 0.017694, 0.040013, 0.046286, 0.063777, 0.085310, 0.140975, 0.182199]
parallel_1d = [0.004458, 0.044833, 0.013258, 0.017673, 0.022389, 0.027871, 0.031376, 0.036579]

serial_2d = [0.005093, 0.020254, 0.045318, 0.080428, 0.077498, 0.110899, 0.161858, 0.203239]
parallel_2d = [0.005148, 0.049896, 0.008472, 0.020703, 0.025467, 0.030714, 0.036915, 0.040972]


speedup_1d = [serial/p1d for serial, p1d in zip(serial_1d, parallel_1d)]
speedup_2d = [serial/p2d for serial, p2d in zip(serial_2d, parallel_2d)]


efficiency_1d = [s/p for s, p in zip(speedup_1d, p_1d)]

efficiency_2d = [s/p for s, p in zip(speedup_2d, p_2d)]



print("1d:")
for w, s, e in zip(W,speedup_1d,efficiency_1d ):
    print(f"w={w}, speedup={s}, effieciency={e}")
print("2d:")
for w, s, e in zip(W,speedup_2d,efficiency_2d ):
    print(f"w={w}, speedup={s}, effieciency={e}")



plt.figure(figsize=(8, 5))
plt.plot(W, speedup_1d, marker='o', label='1D Partitioning')
plt.plot(W, speedup_2d, marker='s', label='2D Partitioning')
plt.xlabel('Isoefficiency Rate W')
plt.ylabel('Speedup')
plt.title('Speedup vs W')
plt.xticks(W)
plt.grid(True)
plt.legend()
plt.tight_layout()
plt.savefig("q4_4_speedup_plot.png", dpi=300, bbox_inches="tight")
plt.show()


plt.figure(figsize=(8, 5))
plt.plot(W, efficiency_1d, marker='o', label='1D Partitioning')
plt.plot(W, efficiency_2d, marker='s', label='2D Partitioning')
plt.xlabel('Isoefficiency Rate W')
plt.ylabel('Efficiency')
plt.title('Efficiency vs W')
plt.xticks(W)
plt.grid(True)
plt.legend()
plt.tight_layout()
plt.savefig("q4_4_efficiency_plot.png", dpi=300, bbox_inches="tight")
plt.show()