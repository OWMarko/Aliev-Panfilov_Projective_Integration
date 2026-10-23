#include "cardiac/pfe.hpp"

namespace cardiac
{

void pas_pfe(State& etat,
             State& precedent,
             State& derivee,
             const Laplacian& laplacien,
             const Parameters& p)
{
    for(int j = 0; j <= p.K; ++j)
    {
        if(j == p.K){precedent = etat;}

        second_membre(etat, derivee, laplacien, p);

        for(std::size_t i = 0; i < etat.u.size(); ++i)
        {
            etat.u[i] += p.micro_pas * derivee.u[i];
            etat.v[i] += p.micro_pas * derivee.v[i];
        }
    }

    //  Y^{n+1} = Y^{K+1} + M (Y^{K+1} - Y^K)
    for(std::size_t i = 0; i < etat.u.size(); ++i)
    {
        etat.u[i] += p.M * (etat.u[i] - precedent.u[i]);
        etat.v[i] += p.M * (etat.v[i] - precedent.v[i]);
    }
}

}
