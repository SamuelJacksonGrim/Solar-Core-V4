#include <cmath>

extern "C" float calculate_pce_degradation_v4(
    float initial_pce = 0.28f,
    float T_stress_K = 298.0f,
    float time_hours = 0.0f,
    float Ea_fast = 0.38f,
    float Ea_slow = 0.92f,
    float tau_fast_ref = 68000.0f,
    float weight_fast = 0.22f,
    float k_slow_ref = 1.8e-8f
) {
    const float k_B = 8.617333262145e-5f;
    const float T_ref = 298.0f;

    float accel_fast = std::exp((Ea_fast / k_B) * (1.0f / T_ref - 1.0f / T_stress_K));
    float accel_slow = std::exp((Ea_slow / k_B) * (1.0f / T_ref - 1.0f / T_stress_K));

    float tau_actual = tau_fast_ref / accel_fast;
    float loss_fast = weight_fast * (1.0f - std::exp(-time_hours / tau_actual));

    float rate_slow = k_slow_ref * accel_slow;
    float loss_slow = (1.0f - weight_fast) * rate_slow * time_hours;
    if (loss_slow > (1.0f - weight_fast)) loss_slow = (1.0f - weight_fast);

    float remaining = initial_pce * (1.0f - loss_fast - loss_slow);
    return remaining < 0.0f ? 0.0f : remaining;
}
