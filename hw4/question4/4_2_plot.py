import matplotlib.pyplot as plt



matrix_sizes = [512, 1024, 2048, 4096]


times_1d_p1 =  [0.004395, 0.017186, 0.037572, 0.149826]

times_2d_p1 = [0.005047, 0.019910, 0.049076, 0.208164]


times_1d_p8 = [0.000929, 0.002297, 0.008833, 0.034839]
times_2d_p8 = [0.001236, 0.002693, 0.010236, 0.040204]

p = 8




speedup_1d = [t1/t8 for t1, t8 in zip(times_1d_p1, times_1d_p8)]
speedup_2d = [t1/t8 for t1, t8 in zip(times_2d_p1, times_2d_p8)]

efficiency_1d = [s / p for s in speedup_1d]

efficiency_2d = [s / p for s in speedup_2d]



print("1d:")
for n, s, e in zip(matrix_sizes,speedup_1d,efficiency_1d ):
    print(f"n={n}, speedup={s}, effieciency={e}")
print("2d:")
for n, s, e in zip(matrix_sizes,speedup_2d,efficiency_2d ):
    print(f"n={n}, speedup={s}, effieciency={e}")

plt.figure(figsize=(8, 5))
plt.plot(matrix_sizes, speedup_1d, marker='o', label='1D Partitioning')
plt.plot(matrix_sizes, speedup_2d, marker='s', label='2D Partitioning')
plt.xlabel('Matrix Size (n x n)')
plt.ylabel('Speedup')
plt.title('Speedup vs Matrix Size (p = 8)')
plt.xticks(matrix_sizes, [f"{n}x{n}" for n in matrix_sizes])
plt.grid(True)
plt.legend()
plt.tight_layout()
plt.savefig("q4_2_speedup_plot.png", dpi=300, bbox_inches="tight")
plt.show()


plt.figure(figsize=(8, 5))
plt.plot(matrix_sizes, efficiency_1d, marker='o', label='1D Partitioning')
plt.plot(matrix_sizes, efficiency_2d, marker='s', label='2D Partitioning')
plt.xlabel('Matrix Size (n x n)')
plt.ylabel('Efficiency')
plt.title('Efficiency vs Matrix Size (p = 8)')
plt.xticks(matrix_sizes, [f"{n}x{n}" for n in matrix_sizes])
plt.grid(True)
plt.legend()
plt.tight_layout()
plt.savefig("q4_2_efficiency_plot.png", dpi=300, bbox_inches="tight")
plt.show()


