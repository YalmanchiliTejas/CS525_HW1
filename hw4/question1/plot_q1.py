import numpy as np
import matplotlib.pyplot as plt

data = np.array([
    [1024,128,0.513446],
    [2048,256,0.553894],
    [4096,512,2.724206],
    [8192,1024,3.400350],
    [16384,2048,4.730296],
    [32768,4096,7.254708],
    [65536,8192,12.521195],
    [102400,12800,18.120003]],dtype=float)

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
