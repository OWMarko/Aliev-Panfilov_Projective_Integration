# Aliev–Panfilov and Projective Integration in 1D

A small C++20 solver for the one-dimensional Aliev–Panfilov reaction–diffusion system, using Projective Forward Euler (PFE) for time integration.

## Model

On the interval \([0,L]\), the excitation \(u\) and recovery \(v\) are governed by

$$
\partial_t u=D\partial_{xx}u+ku(1-u)(u-a)-uv,
$$

$$
\partial_t v=
\left(\varepsilon_0+\frac{\mu_1v}{u+\mu_2}\right)
[-v+ku(1+a-u)].
$$

All variables, spatial coordinates and times are dimensionless. Only \(u\) diffuses, with homogeneous Neumann boundary conditions:

$$
\partial_xu(0,t)=\partial_xu(L,t)=0.
$$

## Numerical Method

The domain is discretized using \(N\) nodes, including both endpoints:

$$
x_i=ih,\qquad h=\frac{L}{N-1}.
$$

The second spatial derivative is approximated by centered finite differences:

$$
(L_hu)_i=\frac{u_{i-1}-2u_i+u_{i+1}}{h^2}.
$$

At the boundaries, ghost-point reflection gives

$$
(L_hu)_0=\frac{2(u_1-u_0)}{h^2},
\qquad
(L_hu)_{N-1}=\frac{2(u_{N-2}-u_{N-1})}{h^2}.
$$

The resulting semi-discrete system is advanced using Projective Forward Euler (PFE).

Each macro-step starts with \(K+1\) explicit Euler micro-steps:

$$
Y^{j+1}=Y^j+\delta t\,F(Y^j),
\qquad j=0,\ldots,K.
$$

The two components are then projected forward:

$$
Y^{n+1}=Y^{K+1}+M(Y^{K+1}-Y^K),
$$

with macro-step size

$$
H=(K+1+M)\delta t.
$$

## Project Structure

```text
include/cardiac/parameters.hpp    model and simulation parameters
include/cardiac/laplacian.hpp     discrete Laplacian declarations
include/cardiac/model.hpp         Aliev–Panfilov model and state
include/cardiac/pfe.hpp           PFE integrator declarations
include/cardiac/simulation.hpp    simulation declarations
include/cardiac/output.hpp        CSV output declarations

src/laplacian.cpp                 finite differences and Neumann boundaries
src/model.cpp                     reaction–diffusion equations
src/pfe.cpp                       Euler micro-steps and projection
src/simulation.cpp                initialization and time loop
src/output.cpp                    CSV export

app/main.cpp                      program entry point
scripts/plot.py                   plots of excitation and recovery
```

## Build

Dependencies: C++20 compiler and CMake 3.16 or newer.

For plotting: Python 3, NumPy and Matplotlib.

From the project root directory:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

On Linux:

```bash
./build/cardiac
```

On Windows :

```powershell
.\build\Release\cardiac.exe
```

To generate the plots:

```bash
python -m pip install numpy matplotlib
python scripts/plot.py
```

## What You Can Modify

The main parameters are defined in `include/cardiac/parameters.hpp`.

The default configuration is:

| Parameter | Value | Description |
|---|---:|---|
| `k` | 8 | Reaction coefficient |
| `a` | 0.15 | Excitation threshold |
| `epsilon0` | 0.002 | Recovery parameter |
| `mu1` | 0.2 | Recovery parameter |
| `mu2` | 0.3 | Recovery parameter |
| `D` | 1 | Diffusion coefficient |
| `L` | 100 | Domain length |
| `N` | 1001 | Number of spatial nodes |
| `T` | 60 | Final simulation time |
| `delta t` | 0.001 | Euler micro-step |
| `K` | 2 | Number of micro-steps before the final Euler step |
| `M` | 2 | Projection parameter |

For these values,

$$
h=0.1,\qquad H=0.005,\qquad
\frac{D\delta t}{h^2}=0.1.
$$

These parameters can be modified, but arbitrary choices do not guarantee numerical stability or accuracy.

## References

- R. R. Aliev and A. V. Panfilov, *A Simple Two-Variable Model of Cardiac Excitation*, Chaos, Solitons & Fractals, 7(3), 293–301 (1996). [DOI](https://doi.org/10.1016/0960-0779%2895%2900089-5).
- C. W. Gear and I. G. Kevrekidis, *Projective Methods for Stiff Differential Equations: Problems with Gaps in Their Eigenvalue Spectrum*, SIAM Journal on Scientific Computing, 24(4), 1091–1106 (2003). [DOI](https://doi.org/10.1137/S1064827501388157).

## License

MIT, see `LICENSE`.