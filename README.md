# Aliev–Panfilov et intégration projective en 1D

Comprendre comment traduire une équation de réaction-diffusion en C++20 et avancer sa solution avec la méthode Projective Forward
Euler (PFE).

## Modèle

Sur l'intervalle `[0,L]`, on calcule l'excitation `u` et la récupération `v` :

$$
\partial_t u=D\partial_{xx}u+ku(1-u)(u-a)-uv,
$$

$$
\partial_t v=\left(\varepsilon_0+\frac{\mu_1v}{u+\mu_2}\right)
             [-v+ku(1+a-u)].
$$

Toutes les variables et coordonnées sont adimensionnées. Seule `u`
diffuse, avec les conditions `u_x(0,t)=u_x(L,t)=0`

Initialement, `v=0`. Une bosse d'excitation est placée à gauche :
`u(x,0)=(1+cos(pi*x/ell))/2` pour `0<=x<ell`, et zéro au-delà, avec `ell=4`.
Sa pente est nulle aux extrémités du support.

## Calcul numérique

La grille contient `N` nœuds, extrémités comprises : `x_i=i*h`,
`h=L/(N-1)`. À l'intérieur,

$$
(L_hu)_i=\frac{u_{i-1}-2u_i+u_{i+1}}{h^2}.
$$

La réflexion des points fictifs donne aux frontières
`(L_hu)_0=2*(u_1-u_0)/h²` et
`(L_hu)_{N-1}=2*(u_{N-2}-u_{N-1})/h²`.
Les deux coefficients du stencil sont construits une fois ; aucune matrice
dense n'est nécessaire.

Pour chaque macro-pas, PFE effectue **K+1 micro-pas** :

$$
Y^{j+1}=Y^j+\delta t\,F(Y^j),\qquad j=0,\ldots,K,
$$

puis projette les deux composantes :

$$
Y^{n+1}=Y^{K+1}+M(Y^{K+1}-Y^K),\qquad H=(K+1+M)\delta t.
$$

Les valeurs sont regroupées dans `include/cardiac/parameters.hpp` :
`k=8`, `a=0.15`, `epsilon0=0.002`, `mu1=0.2`, `mu2=0.3`, `D=1`,
`L=100`, `N=1001`, `T=60`, `delta t=0.001`, `K=2`, `M=2`.
Ainsi `h=0.1`, `H=0.005` et `D*delta t/h²=0.1`. Cette dernière valeur
concerne la diffusion des micro-pas ; elle ne garantit pas la stabilité
du système couplé et de sa projection.

On enregistre l'état initial, tous les 3 000 macro-pas, puis l'état final.
Les temps sont calculés à partir du compteur entier. `T` doit être un
multiple de `H`, à l'arrondi près. Modifier les paramètres puis recompiler.

## Fichiers

| Composant | Rôle |
|---|---|
| `include/cardiac/parameters.hpp` | Paramètres modifiables |
| `include/cardiac/laplacian.hpp`, `src/laplacian.cpp` | Coefficients et application du Laplacien |
| `include/cardiac/model.hpp`, `src/model.cpp` | État à deux vecteurs et second membre AP |
| `include/cardiac/pfe.hpp`, `src/pfe.cpp` | Micro-pas et projection |
| `include/cardiac/simulation.hpp`, `src/simulation.cpp` | Domaine, initialisation et boucle temporelle |
| `include/cardiac/output.hpp`, `src/output.cpp` | Écriture CSV |
| `app/main.cpp` | Lance uniquement la simulation |
| `scripts/plot.py` | Lecture du CSV et deux graphiques |

Les buffers sont alloués avant la boucle temporelle et réutilisés.

## Compiler et lancer

Prérequis : un compilateur C++20 et CMake 3.16 ou plus récent.
Depuis la racine du projet :

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

Sous Linux :

```bash
./build/cardiac
```

Sous Windows, avec le générateur Visual Studio et les outils C++ installés :

```powershell
.\build\Release\cardiac.exe
```

Avec MinGW, l'exécutable se trouve généralement dans `build/cardiac.exe`.
Toujours lancer depuis la racine : le programme crée `results/solution.csv`
avec les colonnes `time,x,u,v`. Une nouvelle exécution remplace ce CSV.

Pour tracer les deux champs :

```bash
python -m pip install numpy matplotlib
python scripts/plot.py
```

Utiliser `python3` si c'est le nom de l'interpréteur sur votre système.
Le script écrit `results/u.png` et `results/v.png`, aux mêmes instants.
`results/` et les dossiers de compilation sont ignorés par Git.

## Vérifications et limites

Compilation et exécution complète vérifiées sous Linux, GCC 13.3 et Python
3.12, avec NumPy 2.3.5 et Matplotlib 3.10.8. Windows n'a pas été exécuté ici.
Les contrôles directs ont porté sur les coefficients de Neumann, le
Laplacien d'une constante, `F(0,0)=0`, la réaction locale, la formule PFE
sur une petite EDO, les valeurs finies et les temps du CSV. Les deux figures
ont été produites et examinées.

Un calcul ponctuel avec `h` et `delta t` divisés par deux donne un écart
de position du front `u=0.5` d'environ `0.087` à `t=60`, par interpolation
linéaire, soit moins d'un pas d'espace initial. Cela ne constitue pas une
étude complète de convergence. Raffiner la grille exige de réexaminer le pas.

PFE reste explicite : stabilité et gain de temps ne sont pas automatiques.
Ce modèle phénoménologique illustre une propagation et une récupération ;
il n'est pas calibré pour une interprétation clinique quantitative.

## Sources et licence

- R. R. Aliev et A. V. Panfilov, *A simple two-variable model of cardiac
  excitation*, Chaos, Solitons & Fractals 7(3), 293–301 (1996).
  [DOI](https://doi.org/10.1016/0960-0779(95)00089-5).
- C. W. Gear et I. G. Kevrekidis, *Projective Methods for Stiff Differential
  Equations: Problems with Gaps in Their Eigenvalue Spectrum*, SIAM J. Sci.
  Comput. 24(4), 1091–1106 (2003).
  [DOI](https://doi.org/10.1137/S1064827501388157).

Projet personnel de Marko Sinadinovic, réécrit avec assistance ChatGPT,
sans affiliation institutionnelle revendiquée. Les modèles et méthodes
sont attribués aux références ci-dessus. La [licence MIT](LICENSE) est
conservée et autorise aussi les utilisations commerciales sous ses conditions.
