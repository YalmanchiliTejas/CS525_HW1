import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit

# Data (actual data points only)
processors = np.array([64, 729, 4096, 15625, 46656], dtype=float)
diameter_2d = np.array([3, 4, 5, 6, 6], dtype=float)
diameter_3d = np.array([3, 4, 5, 5, 6], dtype=float)
diameter_hypercube = np.array([2, 3, 3, 4, 4], dtype=float)

# Single generic model: fit curve from data only (no predefined exponent)
# Power law: y = a * p^b  (both a and b estimated from data)
def power_law(p, a, b):
    return a * (p ** b)

# Fit curves using only the data points (estimate a and b for each network)
params_2d, _ = curve_fit(power_law, processors, diameter_2d)
params_3d, _ = curve_fit(power_law, processors, diameter_3d)
params_hypercube, _ = curve_fit(power_law, processors, diameter_hypercube)

print(f"2D Mesh estimated fit: {params_2d[0]:.3f} * p^{params_2d[1]:.3f}")
print(f"3D Mesh estimated fit: {params_3d[0]:.3f} * p^{params_3d[1]:.3f}")
print(f"Hypercube estimated fit: {params_hypercube[0]:.3f} * p^{params_hypercube[1]:.3f}")
print(f"\nData points - Processors: {processors}")
print(f"2D diameters: {diameter_2d}")
print(f"3D diameters: {diameter_3d}")
print(f"Hypercube diameters: {diameter_hypercube}")

# Smooth curve for plotting: linear x from min to max processor value
p_smooth = np.linspace(processors.min(), processors.max(), 200)

# Create the plot: data points + curves estimated from those points
# Use linear x-axis 0--45000 like the reference plot
plt.figure(figsize=(10, 6))
plt.plot(processors, diameter_2d, 'o', color='blue', markersize=8, label='2D Mesh (data)')
plt.plot(processors, diameter_3d, 's', color='red', markersize=8, label='3D Mesh (data)')
plt.plot(processors, diameter_hypercube, '^', color='green', markersize=8, label='Hypercube (data)')

plt.plot(p_smooth, power_law(p_smooth, *params_2d), '--', color='blue', linewidth=2,
         label=f'2D fit: {params_2d[0]:.2f}*p^{params_2d[1]:.2f}')
plt.plot(p_smooth, power_law(p_smooth, *params_3d), '--', color='red', linewidth=2,
         label=f'3D fit: {params_3d[0]:.2f}*p^{params_3d[1]:.2f}')
plt.plot(p_smooth, power_law(p_smooth, *params_hypercube), '--', color='green', linewidth=2,
         label=f'Hypercube fit: {params_hypercube[0]:.2f}*p^{params_hypercube[1]:.2f}')

plt.xlim(0, 50000)  # Extend to show all data points including 46656
plt.xlabel('Number of Processors', fontsize=12)
plt.ylabel('Diameter', fontsize=12)
plt.title('Diameter vs Number of Processors', fontsize=14)
plt.grid(True, alpha=0.3)
plt.legend(fontsize=10)
plt.tight_layout()
plt.savefig('diameter_comparison.png', dpi=300)
plt.show()

# R² using the estimated curves evaluated at the data points
def r_squared(y_true, y_pred):
    ss_res = np.sum((y_true - y_pred) ** 2)
    ss_tot = np.sum((y_true - np.mean(y_true)) ** 2)
    return 1 - (ss_res / ss_tot)

r2_2d = r_squared(diameter_2d, power_law(processors, *params_2d))
r2_3d = r_squared(diameter_3d, power_law(processors, *params_3d))
r2_hypercube = r_squared(diameter_hypercube, power_law(processors, *params_hypercube))

print(f"\nR² values:")
print(f"2D Mesh: {r2_2d:.4f}")
print(f"3D Mesh: {r2_3d:.4f}")
print(f"Hypercube: {r2_hypercube:.4f}")

