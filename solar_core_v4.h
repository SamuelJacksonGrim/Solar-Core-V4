#ifndef SOLAR_CORE_V4_H
#define SOLAR_CORE_V4_H

extern "C" float calculate_pce_degradation_v4(
    float initial_pce = 0.28f,
    float T_stress_K = 298.0f,
    float time_hours = 0.0f,
    float Ea_fast = 0.38f,
    float Ea_slow = 0.92f,
    float tau_fast_ref = 68000.0f,
    float weight_fast = 0.22f,
    float k_slow_ref = 1.8e-8f
);

#endif // SOLAR_CORE_V4_H
