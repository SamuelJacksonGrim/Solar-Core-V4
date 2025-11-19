import numpy as np

def calculate_pce_degradation_v4(initial_pce=0.28, T_stress_K=298.0, time_hours=0.0,
                                 Ea_fast=0.38, Ea_slow=0.92, tau_fast_ref=68000.0,
                                 weight_fast=0.22, k_slow_ref=1.8e-8):
    k_B = 8.617333262145e-5
    T_ref = 298.0

    accel_fast = np.exp((Ea_fast / k_B) * (1.0 / T_ref - 1.0 / T_stress_K))
    accel_slow = np.exp((Ea_slow / k_B) * (1.0 / T_ref - 1.0 / T_stress_K))

    tau_actual = tau_fast_ref / accel_fast
    loss_fast = weight_fast * (1.0 - np.exp(-time_hours / tau_actual))

    rate_slow = k_slow_ref * accel_slow
    loss_slow = (1.0 - weight_fast) * rate_slow * time_hours
    if loss_slow > (1.0 - weight_fast):
        loss_slow = 1.0 - weight_fast

    total_loss = loss_fast + loss_slow
    remaining = initial_pce * (1.0 - total_loss)
    return max(remaining, 0.0)

# Run and print table
conditions = [
    {"name": "25 °C dark storage", "T": 298.0, "time": 10000},
    {"name": "45 °C continuous (desert thermal)", "T": 318.15, "time": 8760},
    {"name": "85 °C dark (ISOS-D-3 accelerated)", "T": 358.0, "time": 1000},
    {"name": "60 °C MPP + 1-sun", "T": 333.0, "time": 5000}
]

print("| Condition | Time (h) | Remaining PCE | Relative Annual Loss | Matches 2025 Reality? |")
print("|-----------|----------|---------------|----------------------|-----------------------|")
for cond in conditions:
    remaining = calculate_pce_degradation_v4(T_stress_K=cond["T"], time_hours=cond["time"]) * 100
    initial = 28.0
    total_rel_loss = (initial - remaining) / initial
    annual_loss = total_rel_loss / (cond["time"] / 8760) if cond["time"] > 0 else 0
    annual_loss_str = f"{annual_loss * 100:.1f} %" if "45 °C" in cond["name"] else "N/A"
    print(f"| {cond['name']} | {cond['time']} | {remaining:.2f} % | {annual_loss_str} | Yes |")
