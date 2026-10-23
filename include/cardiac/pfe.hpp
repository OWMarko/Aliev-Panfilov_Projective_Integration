#pragma once

#include "cardiac/model.hpp"

namespace cardiac
{

void pas_pfe(State& etat,
             State& precedent,
             State& derivee,
             const Laplacian& laplacien,
             const Parameters& parametres);

}
