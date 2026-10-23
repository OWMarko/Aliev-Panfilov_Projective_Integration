from pathlib import Path

import matplotlib

matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np


def main():
    dossier = Path(__file__).resolve().parents[1] / "results"
    donnees = np.genfromtxt(dossier / "solution.csv", delimiter=",", names=True)
    temps = np.unique(donnees["time"])

    for variable, nom in [("u", "Excitation"), ("v", "Récupération")]:
        figure, axe = plt.subplots(figsize=(8, 4.5))

        for instant in temps:
            profil = donnees[donnees["time"] == instant]
            axe.plot(profil["x"], profil[variable], label=f"t = {instant:g}")

        axe.set_xlabel("x (adimensionné)")
        axe.set_ylabel(f"{variable} (adimensionné)")
        axe.set_title(f"{nom} — Aliev–Panfilov 1D, PFE")
        axe.grid(alpha=0.25)
        axe.legend()
        figure.tight_layout()
        figure.savefig(dossier / f"{variable}.png", dpi=160)
        plt.close(figure)

    print("Figures : results/u.png et results/v.png")


if __name__ == "__main__":
    main()
