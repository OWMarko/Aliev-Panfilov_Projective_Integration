#include "cardiac/output.hpp"

#include <filesystem>
#include <iomanip>
#include <locale>

namespace cardiac
{

std::ofstream ouvrir_sortie()
{
    std::filesystem::create_directories("results");
    std::ofstream fichier;
    fichier.exceptions(std::ios::failbit | std::ios::badbit);
    fichier.open("results/solution.csv");
    fichier.imbue(std::locale::classic());
    fichier << std::setprecision(17) << "time,x,u,v\n";
    return fichier;
}

void enregistrer(std::ofstream& fichier,
                 double temps,
                 const std::vector<double>& x,
                 const State& etat)
{
    for(std::size_t i = 0; i < x.size(); ++i)
    {
        fichier << temps << ',' << x[i] << ',' << etat.u[i] << ',' << etat.v[i] << '\n';
    }
}

}
