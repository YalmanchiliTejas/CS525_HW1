import json
import numpy as np
import matplotlib.pyplot as plt
from collections import defaultdict

one_way_time = {}


with open("results.json", "r") as f:
    

    data = json.load(f)


for proc, time_dict in data.items():

    proc = int(proc)
    one_way_time[proc] = []

    for m_size, time in time_dict.items():
        
        one_way_time[proc].append((int(m_size), time))
    
ts_results = defaultdict(int)
tw_results = defaultdict(int)

for proc in sorted(one_way_time.keys()):

    data_points = sorted(one_way_time[proc])
    message_sizes = np.array([x[0] for x in data_points], dtype=float)
    latencies = np.array([x[1] for x in data_points], dtype=float)

    #solve the T = ts + tw * m equation where tw will be from the slope and ts will be the intercept
    tw , ts = np.polyfit(message_sizes, latencies, 1)

    ts_results[proc] += ts
    tw_results[proc] += tw

print("Estimated values(Average):")
for k , v in ts_results.items():

    ts_results[k] = ts_results[k]
    tw_results[k] = tw_results[k]
    ts = ts_results[k]
    tw = tw_results[k]

    print(f"p = {k}")
    print(f"  ts = {ts:.6f} microseconds")
    print(f"  tw = {tw:.9f} microseconds/byte")

processes = sorted(tw_results.keys())
plot_tws = [tw_results[p] for p in processes]

x_pos = list(range(len(processes)))

plt.figure(figsize=(8, 5))
plt.plot(x_pos, plot_tws, marker="o", linewidth=2, markersize=8)

plt.xticks(x_pos, processes)
plt.xlabel("Number of Processes (p)")
plt.ylabel("tw (microseconds/byte)")
plt.title(" tw vs p")

plt.grid(True, linestyle="--", alpha=0.6)
plt.tight_layout()
plt.savefig("q2_plot.png", dpi=300, bbox_inches="tight")
plt.show()






