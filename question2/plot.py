import matplotlib.pyplot as plt

message_sizes = [1024, 2048, 4096, 8192, 16384, 32768, 65536, 102400]

latencies = {
    2:  [0.530851, 0.565350, 2.465808, 3.097391, 4.202700, 6.181347, 10.210443, 14.801204],
    4:  [0.646853, 0.543952, 2.533400, 3.071249, 4.111910, 6.333506, 9.975493, 14.619350],
    8:  [0.613749, 0.591004, 2.868903, 3.563404, 5.386150, 7.811797, 13.406396, 19.458044],
    16: [0.792646, 0.881207, 4.117191, 4.144299, 5.223155, 8.019757, 14.086103, 21.210504],
    32: [2.049553, 1.908207, 5.844700, 7.595611, 11.228549, 19.703150, 34.985495, 51.581550],
}

plt.figure(figsize=(10, 6))

for p, vals in latencies.items():
    plt.plot(message_sizes, vals, marker='o', label=f"p = {p}")

plt.xlabel("Message Size (bytes)")
plt.ylabel("One-way Latency")
plt.title("One-way Latency vs Message Size for Different Numbers of Processes")
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.savefig("q2_latency_plot.png", dpi=300, bbox_inches="tight")
plt.show()