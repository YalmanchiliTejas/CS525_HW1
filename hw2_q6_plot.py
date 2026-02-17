import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit

# Data
processors = np.array([64, 729, 4096, 15625, 46656])
diameter_data = {
    '2D Mesh': np.array([3, 4, 5, 6, 6]),
    '3D Mesh': np.array([3, 4, 5, 5, 6]),
    'Hypercube': np.array([2, 3, 3, 4, 4])
}
bisection_data = {
    '2D Mesh': np.array([150, 2049, 11955, 46211, 138788]),
    '3D Mesh': np.array([173, 2297, 13653, 53084, 160259]),
    'Hypercube': np.array([45, 192, 952, 4433, 20119])
}
dilation = np.array([3, 4, 5, 5, 6])
congestion = np.array([7, 9, 15, 18, 24])
colors = {'2D Mesh': 'blue', '3D Mesh': 'red', 'Hypercube': 'green'}
markers = {'2D Mesh': 'o', '3D Mesh': 's', 'Hypercube': '^'}

# Power law model: y = a * p^b
power_law = lambda p, a, b: a * (p ** b)
r_squared = lambda y_true, y_pred: 1 - np.sum((y_true - y_pred)**2) / np.sum((y_true - np.mean(y_true))**2)

def plot_metric(data_dict, ylabel, title, filename):
    """Plot metric with fitted curves"""
    params = {name: curve_fit(power_law, processors, values)[0] 
              for name, values in data_dict.items()}
    
    print(f"\n{title} fits:")
    for name, (a, b) in params.items():
        print(f"  {name}: {a:.3f} * p^{b:.3f}")
    
    p_smooth = np.linspace(processors.min(), processors.max(), 200)
    plt.figure(figsize=(10, 6))
    
    for name, values in data_dict.items():
        plt.plot(processors, values, markers[name], color=colors[name], 
                 markersize=8, alpha=0.7, label=f'{name} (data)')
        plt.plot(p_smooth, power_law(p_smooth, *params[name]), '--', 
                 color=colors[name], linewidth=2,
                 label=f'{name} fit: {params[name][0]:.2f}*p^{params[name][1]:.2f}')
    
    plt.xlim(0, 50000)
    plt.xlabel('Number of Processors', fontsize=12)
    plt.ylabel(ylabel, fontsize=12)
    plt.title(title, fontsize=14)
    plt.grid(True, alpha=0.3)
    plt.legend(fontsize=10)
    plt.tight_layout()
    plt.savefig(filename, dpi=300)
    plt.show()
    
    print(f"\n{title} R² values:")
    for name, values in data_dict.items():
        r2 = r_squared(values, power_law(processors, *params[name]))
        print(f"  {name}: {r2:.4f}")

def plot_single_metric(values, ylabel, title, filename, color='blue', marker='o'):
    """Plot single metric with fitted curve"""
    params = curve_fit(power_law, processors, values)[0]
    a, b = params
    
    print(f"\n{title} fit: {a:.3f} * p^{b:.3f}")
    
    p_smooth = np.linspace(processors.min(), processors.max(), 200)
    plt.figure(figsize=(10, 6))
    
    plt.plot(processors, values, marker, color=color, markersize=8, alpha=0.7, label='Data')
    plt.plot(p_smooth, power_law(p_smooth, *params), '--', color=color, linewidth=2,
             label=f'Fit: {a:.2f}*p^{b:.2f}')
    
    plt.xlim(0, 50000)
    plt.xlabel('Number of Processors', fontsize=12)
    plt.ylabel(ylabel, fontsize=12)
    plt.title(title, fontsize=14)
    plt.grid(True, alpha=0.3)
    plt.legend(fontsize=10)
    plt.tight_layout()
    plt.savefig(filename, dpi=300)
    plt.show()
    
    r2 = r_squared(values, power_law(processors, *params))
    print(f"{title} R²: {r2:.4f}")

# Plot diameter
plot_metric(diameter_data, 'Diameter', 'Diameter vs Number of Processors', 'diameter_comparison.png')

# Plot bisection width
plot_metric(bisection_data, 'Bisection Width', 'Bisection Width vs Number of Processors', 'bisection_comparison.png')

# Plot dilation
plot_single_metric(dilation, 'Dilation', 'Dilation vs Number of Processors', 'dilation_comparison.png', color='purple', marker='D')

# Plot congestion
plot_single_metric(congestion, 'Congestion', 'Congestion vs Number of Processors', 'congestion_comparison.png', color='orange', marker='x')

