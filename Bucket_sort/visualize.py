import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

# Lees de timing data
df = pd.read_csv("bucketSortTimings.csv")

# Plot de data
plt.figure(figsize=(10, 5))
plt.scatter(df["n"], df["time"], label="Gemeten tijd", color="blue", alpha=0.6)

# Pas een trendlijn toe
fit = np.polyfit(df["n"], df["time"], 2)  # Polynomiale fit van 2e graad (Bucket Sort is ~O(n + k))
trend = np.poly1d(fit)
plt.plot(df["n"], trend(df["n"]), color="red", label="Trendlijn (Polynomiale fit)")

# Labels en titels
plt.xlabel("Aantal elementen (n)")
plt.ylabel("Tijd (seconden)")
plt.title("Bucket Sort Tijdsanalyse")
plt.legend()
plt.grid()
plt.show()
