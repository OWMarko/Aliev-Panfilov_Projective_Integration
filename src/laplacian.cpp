#include "cardiac/laplacian.hpp"

namespace cardiac
{

Laplacian construire_laplacien(double h)
{
    // Les coefficients sont communs à toute la grille uniforme
    const double inverse_h2 = 1.0 / (h * h);
    return {inverse_h2, 2.0 * inverse_h2};
}

void appliquer_laplacien(const Laplacian& laplacien,
                        const std::vector<double>& u,
                        std::vector<double>& resultat)
{
    const std::size_t n = u.size();

    // Réflexion des points fictifs : u[-1] = u[1] et  u[n] = u[n-2]
    resultat[0] = laplacien.bord * (u[1] - u[0]);

    for(std::size_t i = 1; i < n - 1; ++i)
    {
        resultat[i] = laplacien.interieur * (u[i - 1] - 2.0 * u[i] + u[i + 1]);
    }

    resultat[n - 1] = laplacien.bord * (u[n - 2] - u[n - 1]);
}

}
