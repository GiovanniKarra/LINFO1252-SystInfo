import matplotlib.pyplot as plt
import pandas as pd
import os

nb_de_threads =\
    ["2 Threads", "4 Threads", "8 Threads", "16 Threads", "32 Threads", "64 Threads"]

def perfplot(filename : str, title : str, filename2 : str, filename3 : str = None):
    data : pd.DataFrame = pd.read_csv(filename)
    #essais = [data[f"Essai{i}"] for i in range(1, 6)]
    moyenne = data.transpose().drop("NombreDeThreads").mean()

    data2 : pd.DataFrame = pd.read_csv(filename2)
    moyenne2 = data2.transpose().drop("NombreDeThreads").mean()

    if filename3 is not None:
        data3 : pd.DataFrame = pd.read_csv(filename3)
        moyenne3 = data3.transpose().drop("NombreDeThreads").mean()

    plt.figure(figsize=(15, 7))
    plt.title(title)

    plt.grid(True, color="grey", linestyle=":", linewidth=1)

    #plt.xlabel("Nombre de threads")
    plt.ylabel("temps d'execution [s]")

    plt.xticks(range(1,  len(nb_de_threads)+1), nb_de_threads)

    # for i in range(5):
    #     plt.scatter(range(1,  len(nb_de_threads)+1), essais[i])
    plt.plot(range(1, len(nb_de_threads)+1), moyenne)
    plt.plot(range(1, len(nb_de_threads)+1), moyenne2)
    if filename3 is not None:
        plt.plot(range(1, len(nb_de_threads)+1), moyenne3)

    if filename == "mesures_spinlock.csv":
        plt.legend(["test and set", "test and test and set", "backoff TATAS"])
    else:
        plt.legend(["built-in", "active wait (TATAS)"])

    plt.ylim(ymin=0)

    plt.savefig(f"plots/{hash(filename)%10000}")
    plt.show()


os.system("rm plots/*")

"""PLOT PHILO"""
perfplot("mesures_philo.csv", "Temps d'execution du problème des philosophes",
         "mesures_philo_active.csv")


"""PLOT PRODCONS"""
perfplot("mesures_prodcons.csv", "Temps d'execution du problème des producteurs"\
         " consommateurs", "mesures_prodcons_active.csv")


"""PLOT READWRITE"""
perfplot("mesures_readwrite.csv", "Temps d'execution du problème des lecteurs écrivains",
         "mesures_readwrite_active.csv")


"""PLOT SPINLOCK"""
perfplot("mesures_spinlock.csv", "Temps d'execution avec attente active",
         "mesures_spinlock2.csv", "mesures_backoff.csv")