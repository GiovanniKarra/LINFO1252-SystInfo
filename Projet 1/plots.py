import matplotlib.pyplot as plt
import pandas as pd

nb_de_threads =\
    ["2 Threads", "4 Threads", "8 Threads", "16 Threads", "32 Threads", "64 Threads"]

plt.figure(figsize=(7, 6))
plt.subplots_adjust(hspace=0.5)


"""PLOT PHILO"""
data_philo : pd.DataFrame = pd.read_csv("mesures_philo.csv")
essais_philo = [data_philo[f"Essai{i}"] for i in range(1, 6)]
#moyenne_par_thread = essais.transpose().mean()

plt.subplot(311)

plt.title("Temps d'execution du problème des philosophes")

plt.grid(True, color="grey", linestyle=":", linewidth=1)

#plt.xlabel("Nombre de threads")
plt.ylabel("temps d'execution [s]")

plt.xticks(range(1, len(nb_de_threads)+1), nb_de_threads)

for i in range(5):
    plt.scatter(range(1,  len(nb_de_threads)+1), essais_philo[i])


"""PLOT PRODCONS"""
data_prodcons : pd.DataFrame = pd.read_csv("mesures_prodcons.csv")
essais_prodcons = [data_prodcons[f"Essai{i}"] for i in range(1, 6)]

plt.subplot(312)

plt.title("Temps d'execution du problème du producteur consommateur")

plt.grid(True, color="grey", linestyle=":", linewidth=1)

#plt.xlabel("Nombre de threads")
plt.ylabel("temps d'execution [s]")

plt.xticks(range(1, len(nb_de_threads)), nb_de_threads[:-1])

for i in range(5):
    plt.scatter(range(1, len(nb_de_threads)), essais_prodcons[i])


"""PLOT READWRITE"""
data_readwrite : pd.DataFrame = pd.read_csv("mesures_readwrite.csv")
essais_readwrite = [data_readwrite[f"Essai{i}"] for i in range(1, 6)]

plt.subplot(313)

plt.title("Temps d'execution du problème du lecteur écrivain")

plt.grid(True, color="grey", linestyle=":", linewidth=1)

#plt.xlabel("Nombre de threads")
plt.ylabel("temps d'execution [s]")

plt.xticks(range(1,  len(nb_de_threads)+1), nb_de_threads)

for i in range(5):
    plt.scatter(range(1,  len(nb_de_threads)+1), essais_readwrite[i])

plt.show()