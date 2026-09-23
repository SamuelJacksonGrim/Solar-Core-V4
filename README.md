# solar-core-v4

[![License: AGPL-3.0-only](https://img.shields.io/badge/license-AGPL--3.0--only-blue)](LICENSE)
[![dual-license](https://img.shields.io/badge/dual--license-AGPL--3.0--only%20or%20commercial-blueviolet)](LICENSING.md)
[![C++](https://img.shields.io/badge/C%2B%2B-00599C?logo=cplusplus&logoColor=white)](https://isocpp.org)
[![Python](https://img.shields.io/badge/Python-3776AB?logo=python&logoColor=white)](https://www.python.org)


Open-source dual-mode thermal degradation model for passivated triple-cation CsFAMA perovskite solar cells.

## Overview
This is a lightweight C++ kernel for simulating PCE (Power Conversion Efficiency) degradation under thermal stress. It's calibrated to 2024–2025 literature for high-stability perovskites (e.g., Oxford PV, NREL, Saule Technologies, Microquanta). The model uses a fast exponential (ion migration/interface) + slow linear-Arrhenius (bulk decomposition) approach.

- Matches real desert annual loss rates (~6–10% at 45°C equivalent).
- Blazing fast: <1 ns per call on modern hardware.
- Defaults tuned for 2025-era commercial cells (>95% retention after 10k hours at 25°C).

Use it for Monte Carlo fleet simulations, LCOE modeling, or BESS optimization.

## Usage
Compile into a shared lib (e.g., `g++ -shared -fPIC -o libsolar_core_v4.so solar_core_v4.cpp`).

Call from C/C++ or Python (via ctypes).

Example (C++):
```cpp
#include "solar_core_v4.h"
float remaining = calculate_pce_degradation_v4(0.28f, 318.15f, 8760.0f);  // 45°C, 1 year
