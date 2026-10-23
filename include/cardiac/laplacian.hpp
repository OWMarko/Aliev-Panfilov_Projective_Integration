#pragma once

#include <vector>

namespace cardiac
{

struct Laplacian
{
    double interieur;
    double bord;
};

Laplacian construire_laplacien(double h);
void appliquer_laplacien(const Laplacian& laplacien,
                        const std::vector<double>& u,
                        std::vector<double>& resultat);

}
