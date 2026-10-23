#include "cardiac/simulation.hpp"

#include "cardiac/model.hpp"
#include "cardiac/output.hpp"
#include "cardiac/pfe.hpp"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <numbers>
#include <stdexcept>

namespace cardiac
{

int run_simulation()
{
    try
    {
        const Parameters p;

        if(p.nombre_noeuds < 3 || !(p.longueur > 0.0) || !(p.micro_pas > 0.0)
           || !(p.temps_final > 0.0) || p.K < 0 || p.M < 0 || p.frequence_sortie < 1
           || !(p.largeur_stimulus > 0.0 && p.largeur_stimulus < p.longueur)
           || !(p.k > 0.0 && p.a > 0.0 && p.a < 1.0 && p.epsilon0 > 0.0
                && p.mu1 >= 0.0 && p.mu2 > 0.0 && p.D >= 0.0))
        {
            throw std::invalid_argument("Parametres incompatibles avec la simulation.");
        }

        const double h = p.longueur / (p.nombre_noeuds - 1);
        const double H = (p.K + 1.0 + p.M) * p.micro_pas;
        const double quotient = p.temps_final / H;

        if(!std::isfinite(quotient) || quotient < 1.0 || quotient > std::numeric_limits<int>::max())
        {
            throw std::invalid_argument("Nombre de macro-pas invalide.");
        }

        const int nombre_pas = static_cast<int>(std::llround(quotient));
        const double arrondi = 64.0 * std::numeric_limits<double>::epsilon() * std::max(1.0, p.temps_final);

        if(std::abs(nombre_pas * H - p.temps_final) > arrondi)
        {
            throw std::invalid_argument("Le temps final doit etre un multiple du macro-pas H.");
        }

        std::vector<double> x(p.nombre_noeuds);
        State etat{std::vector<double>(p.nombre_noeuds, 0.0), std::vector<double>(p.nombre_noeuds, 0.0)};
        State precedent = etat;
        State derivee = etat;
        const Laplacian laplacien = construire_laplacien(h);

        for(int i = 0; i < p.nombre_noeuds; ++i)
        {
            x[i] = i * h;

            if(x[i] < p.largeur_stimulus)
            {
                etat.u[i] = 0.5 * (1.0 + std::cos(std::numbers::pi * x[i] / p.largeur_stimulus));
            }
        }

        std::ofstream fichier = ouvrir_sortie();
        enregistrer(fichier, 0.0, x, etat);

        for(int n = 1; n <= nombre_pas; ++n)
        {
            pas_pfe(etat, precedent, derivee, laplacien, p);

            for(int i = 0; i < p.nombre_noeuds; ++i)
            {
                if(!std::isfinite(etat.u[i]) || !std::isfinite(etat.v[i]) || etat.u[i] + p.mu2 <= 0.0)
                {
                    throw std::runtime_error("Etat invalide : reduire le pas et verifier les parametres.");
                }
            }

            if(n % p.frequence_sortie == 0 || n == nombre_pas)
            {
                enregistrer(fichier, n * H, x, etat);
            }
        }

        fichier.close();
        std::cout << "Resultats : results/solution.csv\n";
        return 0;
    }
    catch(const std::exception& erreur)
    {
        std::cerr << "Erreur : " << erreur.what() << '\n';
        return 1;
    }
}

}
