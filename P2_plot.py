
import numpy as np
import matplotlib.pyplot as plt
import pandas as pd

data = pd.read_csv("values_P2.csv")
print(data)

inputSize = data.iloc[:, 2]


grouped = data.groupby(['InputSize'])
print(grouped)

uniqueInputs = inputSize.unique()
print(uniqueInputs)

for i in uniqueInputs:
    indiv = grouped.get_group(i)
    runtime = indiv.iloc[:, 0]
    threads = indiv.iloc[:, 1]
    plt.plot(threads, runtime, label=str(i))


plt.xlabel('No of threads')
plt.ylabel('Time [ns]')
plt.title('Time vs Threads')
plt.legend()
plt.show()
