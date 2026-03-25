import numpy as np
import matplotlib.pyplot as plt

data = np.array([[1000,125,1.854002],
[5000,625,11.642551],
[10000,1250,12.922895],
[15000,1875,8.477151],
[20000,2500,8.893204],
[25000,3125,10.673749],
[30000,3750,11.799502],
[35000,4375,12.861407],
[40000,5000,13.926959],
[45000,5625,14.980006],
[50000,6250,16.473842],
[55000,6875,16.925955],
[60000,7500,18.013942],
[65000,8125,19.086945],
[70000,8750,20.947206],
[75000,9375,22.078395],
[80000,10000,23.135555],
[85000,10625,24.219954],
[90000,11250,25.307703],
[95000,11875,26.844192],
[100000,12500,27.951741]], dtype=float)

byt = data[:,0]
words = data[:,1]
time_us = data[:,2]
#fitting to ts + tw*m
tw, ts = np.polyfit(words, time_us, 1)
time_fit = tw * words + ts
sqs_res = np.sum((time_us - time_fit) ** 2)
sqs_tot = np.sum((time_us - np.mean(time_us)) ** 2)
r2 = 1 - sqs_res / sqs_tot

print(f"Estimated ts = {ts:.6f} us")
print(f"Estimated tw = {tw:.8f} us/word")
print(f"R^2 = {r2:.4f}")
w_smooth = np.linspace(words.min(), words.max(), 300)
t_smooth = tw * w_smooth + ts
plt.figure(figsize=(8, 5))
plt.scatter(words, time_us, s=60, alpha=0.8, label="Measured one-way time")
plt.plot(w_smooth, t_smooth, "r--",label=f"Fit: t = {ts:.3f} + {tw:.6f}·m (us), R²={r2:.3f}")
plt.xlabel("Message length m (words)")
plt.ylabel("Communication time t (us)")
plt.title("Q1: Communication time vs message length")
plt.grid(True, alpha=0.3)
plt.legend()
plt.tight_layout()
plt.savefig("q1_time_fit.png", dpi=300)
plt.show()
