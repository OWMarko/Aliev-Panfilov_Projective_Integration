#pragma once

#include "cardiac/model.hpp"

#include <fstream>
#include <vector>

namespace cardiac
{

std::ofstream ouvrir_sortie();
void enregistrer(std::ofstream& fichier,
                 double temps,
                 const std::vector<double>& x,
                 const State& etat);

}
