#pragma once

namespace cardiac
{

struct Parameters
{
    double k = 8.0;
    double a = 0.15;
    double epsilon0 = 0.002;
    double mu1 = 0.2;
    double mu2 = 0.3;
    double D = 1.0;

    double longueur = 100.0;
    int nombre_noeuds = 1001;
    double largeur_stimulus = 4.0;
    double temps_final = 60.0;

    double micro_pas = 0.001;
    int K = 2;
    int M = 2;
    int frequence_sortie = 3000; // Nombre de macro-pas entre deux enregistrements.
};

}
