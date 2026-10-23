#include "cardiac/model.hpp"

#include <cmath>
#include <stdexcept>

namespace cardiac
{

void second_membre(const State& etat,
                   State& derivee,
                   const Laplacian& laplacien,
                   const Parameters& p)
{
    appliquer_laplacien(laplacien, etat.u, derivee.u);

    for(std::size_t i = 0; i < etat.u.size(); ++i)
    {
        const double u = etat.u[i];
        const double v = etat.v[i];

        if(!std::isfinite(u) || !std::isfinite(v) || u + p.mu2 <= 0.0)
        {
            throw std::runtime_error("Etat non fini ou hors du domaine u + mu2 > 0.");
        }

        const double epsilon = p.epsilon0 + p.mu1 * v / (u + p.mu2);

        // Seule l'excitation u diffuse et v suit la cinétique locale
        derivee.u[i] = p.D * derivee.u[i] + p.k * u * (1.0 - u) * (u - p.a) - u * v;
        derivee.v[i] = epsilon * (-v + p.k * u * (1.0 + p.a - u));
    }
}

}
