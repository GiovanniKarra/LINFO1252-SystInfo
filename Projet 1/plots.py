import matplotlib.pyplot as plt
from matplotlib.backends.backend_pdf import PdfPages
import pandas as pd
import os

nb_de_threads =\
    ["2 Threads", "4 Threads", "8 Threads", "16 Threads", "32 Threads", "64 Threads"]

def perfplot(filename : str, title : str, filename2 : str, filename3 : str = None):
    data : pd.DataFrame = pd.read_csv("mesures/"+filename)
    #essais = [data[f"Essai{i}"] for i in range(1, 6)]
    moyenne = data.transpose().drop("NombreDeThreads").mean()
    ecart = data.transpose().drop("NombreDeThreads").std()

    data2 : pd.DataFrame = pd.read_csv("mesures/"+filename2)
    moyenne2 = data2.transpose().drop("NombreDeThreads").mean()
    ecart2 = data2.transpose().drop("NombreDeThreads").std()

    if filename3 is not None:
        data3 : pd.DataFrame = pd.read_csv("mesures/"+filename3)
        moyenne3 = data3.transpose().drop("NombreDeThreads").mean()
        ecart3 = data3.transpose().drop("NombreDeThreads").std()

    plt.figure(figsize=(15, 7))
    plt.title(title)

    plt.grid(True, color="grey", linestyle=":", linewidth=1)

    #plt.xlabel("Nombre de threads")
    plt.ylabel("temps d'execution [s]")

    plt.xticks(range(1,  len(nb_de_threads)+1), nb_de_threads)

    # for i in range(5):
    #     plt.scatter(range(1,  len(nb_de_threads)+1), essais[i])
    plt.plot(range(1, len(nb_de_threads)+1), moyenne)
    plt.fill_between(range(1, len(nb_de_threads)+1), moyenne+ecart, moyenne-ecart, alpha=0.4)
    plt.plot(range(1, len(nb_de_threads)+1), moyenne2)
    plt.fill_between(range(1, len(nb_de_threads)+1), moyenne2+ecart2, moyenne2-ecart2, alpha=0.4)
    if filename3 is not None:
        plt.plot(range(1, len(nb_de_threads)+1), moyenne3)
        plt.fill_between(range(1, len(nb_de_threads)+1), moyenne3+ecart3, moyenne3-ecart3, alpha=0.4)

    if filename == "mesures_spinlock.csv":
        plt.legend(["test and set", "moyenne ± écrart type", "test and test and set",
                    "moyenne ± écrart type", "backoff TATAS", "moyenne ± écrart type"])
    else:
        plt.legend(["POSIX", "moyenne ± écrart type", "attente active (TATAS)",
                    "moyenne ± écrart type"])

    plt.ylim(ymin=0)

    pp = PdfPages(f"plots/{hash(filename)%10000}.pdf")
    pp.savefig()
    pp.close()
    plt.show()


os.system("rm plots/*")

plt.rcParams.update({'font.size': 18})

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