import matplotlib.pyplot as plt

# pBST data
threads = [1, 2, 4, 8, 16, 32, 64, 128]
pbst_times = [113.01, 64.62,  58.37, 59.01, 57.39, 57.17,  56.88, 57.01]

# LBST data
lbst_time = 2.76
lbst_times = [lbst_time] * len(threads)

plt.figure(figsize=(10, 6))

plt.plot(threads, pbst_times, marker='o', label='pBST')
plt.plot(threads, lbst_times, marker='s', linestyle='--', label='LBST')

plt.xscale('log', base=2)
plt.xticks(threads, threads)

plt.xlabel('Number of Threads')
plt.ylabel('Execution Time (seconds)')
plt.title('Execution Time vs Number of Threads')
plt.legend()
plt.grid(True)

plt.tight_layout()
plt.savefig("execution_times.png", dpi=300)