import re
from collections import defaultdict

import matplotlib.pyplot as plt
serial = """100000,0.142074
1000000,1.400953
10000000,12.203427"""
RAW = r"""
mpirun -np 2 ./run 100
100000,2,0.142074,0.163367,0.869661,0.434831
sorted
mpirun -np 2 ./run 1000
1000000,2,1.400953,1.434827,0.976392,0.488196
sorted
mpirun -np 2 ./run 10000
10000000,2,12.203427,8.383536,1.455642,0.727821
sorted
mpirun -np 4 ./run 100
100000,4,0.142074,0.107543,1.321090,0.330273
sorted
mpirun -np 4 ./run 1000
1000000,4,1.400953,1.090640,1.284524,0.321131
sorted
mpirun -np 4 ./run 10000
10000000,4,12.203427,10.444567,1.168399,0.292100
sorted
mpirun -np 8 ./run 100
100000,8,0.142074,0.087829,1.617619,0.202202
sorted
mpirun -np 8 ./run 1000
1000000,8,1.400953,0.747258,1.874792,0.234349
sorted
mpirun -np 8 ./run 10000
10000000,8,12.203427,5.687031,2.145834,0.268229
sorted
mpirun -np 16 ./run 100
100000,16,0.142074,0.047289,3.004384,0.187774
sorted
mpirun -np 16 ./run 1000
1000000,16,1.400953,0.612768,2.286269,0.142892
sorted
mpirun -np 16 ./run 10000
10000000,16,12.203427,7.706842,1.583454,0.098966
sorted
mpirun -np 32 ./run 100
100000,32,0.142074,0.067562,2.102865,0.065715
sorted
mpirun -np 32 ./run 1000
1000000,32,1.400953,0.332310,4.215802,0.131744
sorted
mpirun -np 32 ./run 10000
10000000,32,12.203427,2.968062,4.111581,0.128487
sorted
"""

# Parse CSV lines: N,p,serial,parallel,speedup,efficiency
rows = []
for line in RAW.splitlines():
    line = line.strip()
    if not line or line.startswith("mpirun") or line == "sorted":
        continue
    if not re.match(r"^\d+,\d+,[0-9.]+,[0-9.]+,[0-9.]+,[0-9.]+$", line):
        continue
    N_s, p_s, serial_s, parallel_s, speedup_s, eff_s = line.split(",")
    rows.append(
        {
            "N": int(N_s),
            "p": int(p_s),
            "serial": float(serial_s),
            "parallel": float(parallel_s),
            "speedup": float(speedup_s),
            "eff": float(eff_s),
        }
    )

if not rows:
    raise SystemExit("No data rows parsed. Check RAW formatting.")

# Organize
by_p = defaultdict(list)  # p -> list of (N, speedup)
by_N = defaultdict(list)  # N -> list of (p, speedup)
for r in rows:
    by_p[r["p"]].append((r["N"], r["speedup"]))
    by_N[r["N"]].append((r["p"], r["speedup"]))

# Plot 1: Speedup vs list size (fix p, X-axis N)
plt.figure(figsize=(8, 5))
for p in sorted(by_p):
    pts = sorted(by_p[p], key=lambda t: t[0])
    Ns = [x for x, _ in pts]
    Ss = [y for _, y in pts]
    plt.plot(Ns, Ss, marker="o", label=f"p={p}")
plt.xscale("log")
plt.xlabel("List size N (elements)")
plt.ylabel("Speedup = Tserial / Tparallel")
plt.title("Q3: Speedup vs list size (fixed p)")
plt.grid(True, alpha=0.3)
plt.legend()
plt.tight_layout()
plt.savefig("q3_speedup_vs_N.png", dpi=300)

# Plot 2: Speedup vs p (fix N, X-axis p)
plt.figure(figsize=(8, 5))
for N in sorted(by_N):
    pts = sorted(by_N[N], key=lambda t: t[0])
    ps = [x for x, _ in pts]
    Ss = [y for _, y in pts]
    plt.plot(ps, Ss, marker="o", label=f"N={N}")
plt.xscale("log", base=2)
plt.xticks(sorted(by_p.keys()), [str(p) for p in sorted(by_p.keys())])
plt.xlabel("Processors p")
plt.ylabel("Speedup = Tserial / Tparallel")
plt.title("Q3: Speedup vs p (fixed N)")
plt.grid(True, alpha=0.3)
plt.legend()
plt.tight_layout()
plt.savefig("q3_speedup_vs_p.png", dpi=300)

plt.show()
print("Saved: q3_speedup_vs_N.png, q3_speedup_vs_p.png")
