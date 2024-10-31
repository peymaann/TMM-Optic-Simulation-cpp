import matplotlib.pyplot as plt
import pandas as pd
resolution = 400
data = pd.read_csv('results/total_Generation.dat', comment='#', header=None)
plt.figure(figsize=(10, 6))
plt.plot(data[0], data[1], linewidth=3, label=f'Generation Profile')

plt.xlabel('X [nm]')
plt.ylabel('Generation Rtae [1/cm^3]')
plt.legend()
plt.savefig('figs/Generation_rate.png', dpi=resolution)

data = pd.read_csv('results/TRA.dat', comment='#', header=None)
plt.figure(figsize=(10, 6))
plt.plot(data[0], data[1], linewidth=3, label=f'Transmission')
plt.plot(data[0], data[2], linewidth=3, label=f'Reflection')
plt.plot(data[0], data[3], linewidth=3, label=f'Absoprtion')
plt.xlabel('X [nm]')
plt.ylabel('T/R/A [[1]')
plt.legend()
plt.savefig('figs/TRA.png', dpi=resolution)
