#include <cmath>

extern "C" float calculate_pce_degradation_v5(
    float initial_pce = 0.28f,
    float T_stress_K = 298.0f,
    float time_hours = 0.0f,
    // Thermal (v4)
    float Ea_fast = 0.38f, float Ea_slow = 0.92f,
    float tau_fast_ref = 68000.0f, float weight_fast = 0.22f,
    float k_slow_ref = 1.8e-8f,
    // Photo pathways
    float irradiance_suns = 1.0f, float duty_mpp = 0.7f,
    float uv_fraction = 0.03f, float pO2 = 0.2f, float RH = 0.2f,
    float alpha_uv = 4.0f, float alpha_o2 = 2.0f, float alpha_h2o = 3.0f,
    float Ea_photo_fast = 0.30f, float tau_photo_ref = 40000.0f,
    float weight_photo_fast = 0.08f,
    float k_photo_slow_ref = 2.5e-8f, float weight_photo_slow = 0.10f
) {
    const float k_B = 8.617333262145e-5f;
    const float T_ref = 298.0f;

    // Thermal
    float accel_fast = std::exp((Ea_fast / k_B) * (1.0f / T_ref - 1.0f / T_stress_K));
    float accel_slow = std::exp((Ea_slow / k_B) * (1.0f / T_ref - 1.0f / T_stress_K));

    float tau_actual = tau_fast_ref / accel_fast;
    float loss_fast = weight_fast * (1.0f - std::exp(-time_hours / tau_actual));

    float rate_slow = k_slow_ref * accel_slow;
    float loss_slow = (1.0f - weight_fast) * rate_slow * time_hours;
    if (loss_slow > (1.0f - weight_fast)) loss_slow = (1.0f - weight_fast);

    // Light scaling
    float F_light = irradiance_suns * duty_mpp;
    float F_chem = (1.0f + alpha_uv * uv_fraction) *
                   (1.0f + alpha_o2 * pO2) *
                   (1.0f + alpha_h2o * RH);

    // Photo-fast
    float accel_photo_fast = std::exp((Ea_photo_fast / k_B) * (1.0f / T_ref - 1.0f / T_stress_K));
    float tau_photo = tau_photo_ref / (accel_photo_fast * F_light * F_chem);
    if (tau_photo < 1.0f) tau_photo = 1.0f;

    float loss_photo_fast = weight_photo_fast * (1.0f - std::exp(-time_hours / tau_photo));

    // Photo-slow
    float rate_photo_slow = k_photo_slow_ref * accel_photo_fast * F_light * F_chem;
    float loss_photo_slow = weight_photo_slow * rate_photo_slow * time_hours;
    if (loss_photo_slow > weight_photo_slow) loss_photo_slow = weight_photo_slow;

    // Total
    float total_loss = loss_fast + loss_slow + loss_photo_fast + loss_photo_slow;
    float remaining = initial_pce * (1.0f - total_loss);
    return remaining < 0.0f ? 0.0f : remaining;
}
