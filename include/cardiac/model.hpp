#pragma once

#include "cardiac/laplacian.hpp"
#include "cardiac/parameters.hpp"

#include <vector>

namespace cardiac
{

struct State
{
    std::vector<double> u;
    std::vector<double> v;
};

void second_membre(const State& etat,
                   State& derivee,
                   const Laplacian& laplacien,
                   const Parameters& parametres);

}
