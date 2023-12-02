import matplotlib.pyplot as plt
import pandas as pd

nb_de_threads =\
    ["2 Threads", "4 Threads", "8 Threads", "16 Threads", "32 Threads", "64 Threads"]

def perfplot(filename : str, title : str, position : int):
    data : pd.DataFrame = pd.read_csv(filename)
    essais = [data[f"Essai{i}"] for i in range(1, 6)]
    moyenne = data.transpose().drop("NombreDeThreads").mean()

    plt.subplot(position)

    plt.title(title)

    plt.grid(True, color="grey", linestyle=":", linewidth=1)

    #plt.xlabel("Nombre de threads")
    plt.ylabel("temps d'execution [s]")

    plt.xticks(range(1,  len(nb_de_threads)+1), nb_de_threads)

    # for i in range(5):
    #     plt.scatter(range(1,  len(nb_de_threads)+1), essais[i])
    plt.plot(range(1, len(nb_de_threads)+1), moyenne)

    plt.ylim(ymin=0)


plt.figure(figsize=(15, 7))
plt.subplots_adjust(hspace=0.5)


"""PLOT PHILO"""
perfplot("mesures_philo.csv", "Temps d'execution du problème des philosophes", 221)


"""PLOT PRODCONS"""
perfplot("mesures_prodcons.csv", "Temps d'execution du problème des producteurs"\
         " consommateurs", 222)


"""PLOT READWRITE"""
perfplot("mesures_readwrite.csv", "Temps d'execution du problème des lecteurs écrivains", 223)


"""PLOT SPINLOCK"""
perfplot("mesures_spinlock.csv", "Temps d'execution avec attente active test-and-set", 224)

plt.show()