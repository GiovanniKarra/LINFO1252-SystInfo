import matplotlib.pyplot as plt
import pandas as pd

data_philo : pd.DataFrame = pd.read_csv("mesures_philo.csv")
nb_de_threads = data_philo["NombreDeThreads"]
essais = [data_philo[f"Essai{i}"] for i in range(1, 6)]
#moyenne_par_thread = essais.transpose().mean()

plt.figure()

plt.title("Temps d'execution du problème des philosophes")

plt.grid(True, color="grey", linestyle=":", linewidth=1)

plt.xlabel("Nombre de threads")
plt.ylabel("temps d'execution [s]")

plt.xticks([2, 4, 8, 16, 32, 64])

for i in range(5):
    plt.scatter(nb_de_threads, essais[i])

plt.show()