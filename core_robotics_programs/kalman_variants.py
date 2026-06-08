"""
Kalman Filter Variants — Linear KF, EKF, UKF
=============================================
Linear KF  → use when: dynamics AND sensor are both linear
EKF        → use when: either is nonlinear — linearize with Jacobians each step
UKF        → use when: strongly nonlinear — propagate sigma points, no Jacobians needed

Scenario for EKF / UKF: pendulum
  State:    x = [angle θ, angular velocity θ̇]
  Dynamics: nonlinear — θ̈ = -(g/L)*sin(θ)  ← the sin makes it nonlinear
  Sensor:   measures angle only (linear measurement)
"""

import numpy as np
import matplotlib.pyplot as plt

np.random.seed(42)

# ═══════════════════════════════════════════════════════════════════════════════
# PART 1 — LINEAR KALMAN FILTER
# Problem: 1D robot moving at constant velocity, noisy position sensor
# ═══════════════════════════════════════════════════════════════════════════════

def linear_kf():
    dt = 0.1
    steps = 100

    # State = [position, velocity]
    A = np.array([[1, dt],   # new_pos = pos + vel*dt
                  [0,  1]])  # new_vel = vel  (constant velocity)

    H = np.array([[1, 0]])   # sensor measures position only

    Q = np.eye(2) * 0.01    # small process noise — we trust our model
    R = np.array([[4.0]])   # measurement noise variance (sensor std = 2m)

    x = np.array([0.0, 0.0])  # initial state [pos=0, vel=0]
    P = np.eye(2)              # initial uncertainty

    true_positions, measurements, estimates = [], [], []

    for i in range(steps):
        true_pos = dt * i
        z = true_pos + np.random.normal(0, 2.0)  # noisy sensor reading

        # ── PREDICT ──────────────────────────────────────────────────────────
        x = A @ x                    # push state forward with motion model
        P = A @ P @ A.T + Q          # uncertainty grows

        # ── UPDATE ───────────────────────────────────────────────────────────
        S = H @ P @ H.T + R          # innovation covariance (expected surprise)
        K = P @ H.T @ np.linalg.inv(S)  # Kalman gain: how much to trust sensor
        x = x + K @ (np.array([z]) - H @ x)  # correct estimate
        P = (np.eye(2) - K @ H) @ P           # uncertainty shrinks

        true_positions.append(true_pos)
        measurements.append(z)
        estimates.append(x[0])

    return true_positions, measurements, estimates


# ═══════════════════════════════════════════════════════════════════════════════
# PART 2 — EXTENDED KALMAN FILTER (EKF)
# Problem: pendulum — nonlinear dynamics because of sin(θ)
# Key idea: linearize the nonlinear function at the current estimate each step
#           using its Jacobian (first-order Taylor expansion)
# ═══════════════════════════════════════════════════════════════════════════════

def ekf():
    dt = 0.05
    g = 9.81
    L = 1.0      # pendulum length in meters
    steps = 200

    # ── NONLINEAR DYNAMICS f(x) ──────────────────────────────────────────────
    # True physics: θ̈ = -(g/L)*sin(θ)
    # Discretized:  θ_new  = θ + θ̇*dt
    #               θ̇_new = θ̇ - (g/L)*sin(θ)*dt
    def f(x):
        theta, theta_dot = x
        return np.array([
            theta + theta_dot * dt,
            theta_dot - (g / L) * np.sin(theta) * dt
        ])

    # ── JACOBIAN OF f (the linearization) ────────────────────────────────────
    # df/dx evaluated at current x — a 2×2 matrix
    # Row 0: d(θ_new)/d[θ, θ̇]     = [1, dt]
    # Row 1: d(θ̇_new)/d[θ, θ̇]    = [-(g/L)*cos(θ)*dt, 1]
    # This changes every step because cos(θ) changes — that's the EKF key step
    def F_jacobian(x):
        theta = x[0]
        return np.array([
            [1,                         dt],
            [-(g / L) * np.cos(theta) * dt, 1]
        ])

    # ── SENSOR MODEL ─────────────────────────────────────────────────────────
    # Linear: we measure angle directly → H is constant
    H = np.array([[1, 0]])
    R = np.array([[0.05]])   # sensor noise variance for angle measurement

    Q = np.eye(2) * 0.001   # small process noise
    x = np.array([0.8, 0.0])  # start: 0.8 rad swing, zero velocity
    P = np.eye(2) * 0.1

    true_angles, measurements, estimates = [], [], []

    # Simulate true pendulum (no noise — this is ground truth)
    x_true = np.array([0.8, 0.0])

    for _ in range(steps):
        x_true = f(x_true)   # true physics (no noise)
        z = x_true[0] + np.random.normal(0, np.sqrt(R[0, 0]))  # noisy angle reading

        # ── EKF PREDICT ──────────────────────────────────────────────────────
        F = F_jacobian(x)    # linearize f at current estimate
        x = f(x)             # propagate state through the NONLINEAR function
        P = F @ P @ F.T + Q  # propagate covariance through the LINEARIZED Jacobian

        # ── EKF UPDATE ───────────────────────────────────────────────────────
        # Same as linear KF — sensor is linear so H is constant here
        S = H @ P @ H.T + R
        K = P @ H.T @ np.linalg.inv(S)
        x = x + K @ (np.array([z]) - H @ x)
        P = (np.eye(2) - K @ H) @ P

        true_angles.append(x_true[0])
        measurements.append(z)
        estimates.append(x[0])

    return true_angles, measurements, estimates


# ═══════════════════════════════════════════════════════════════════════════════
# PART 3 — UNSCENTED KALMAN FILTER (UKF)
# Same pendulum problem — but instead of linearizing with Jacobians,
# we pick carefully chosen "sigma points", push them through the nonlinear
# function, and reconstruct the mean and covariance from the results.
# No Jacobian needed. Better accuracy for strongly nonlinear systems.
# ═══════════════════════════════════════════════════════════════════════════════

def ukf():
    dt = 0.05
    g = 9.81
    L = 1.0
    steps = 200
    n = 2          # state dimension

    # ── UKF PARAMETERS ───────────────────────────────────────────────────────
    alpha = 1e-3   # spread of sigma points around mean (small = tight cluster)
    kappa = 0.0    # secondary scaling (usually 0)
    beta  = 2.0    # 2 is optimal for Gaussian distributions
    lam = alpha**2 * (n + kappa) - n  # composite scaling parameter

    # ── WEIGHTS for reconstructing mean and covariance from sigma points ──────
    Wm = np.full(2 * n + 1, 1 / (2 * (n + lam)))  # mean weights
    Wc = np.full(2 * n + 1, 1 / (2 * (n + lam)))  # covariance weights
    Wm[0] = lam / (n + lam)
    Wc[0] = lam / (n + lam) + (1 - alpha**2 + beta)  # extra term for 0th point

    def f(x):
        theta, theta_dot = x
        return np.array([
            theta + theta_dot * dt,
            theta_dot - (g / L) * np.sin(theta) * dt
        ])

    H = np.array([[1, 0]])
    R = np.array([[0.05]])
    Q = np.eye(2) * 0.001
    x = np.array([0.8, 0.0])
    P = np.eye(2) * 0.1

    true_angles, measurements, estimates = [], [], []
    x_true = np.array([0.8, 0.0])

    for _ in range(steps):
        x_true = f(x_true)
        z = x_true[0] + np.random.normal(0, np.sqrt(R[0, 0]))

        # ── UKF PREDICT ──────────────────────────────────────────────────────
        # Step 1: generate 2n+1 sigma points around current estimate
        # sqrt_P columns define the spread direction and magnitude
        sqrt_P = np.linalg.cholesky((n + lam) * P)  # matrix square root
        sigma_pts = np.zeros((2 * n + 1, n))
        sigma_pts[0] = x                             # center point
        for i in range(n):
            sigma_pts[i + 1]     = x + sqrt_P[:, i]  # +direction
            sigma_pts[i + 1 + n] = x - sqrt_P[:, i]  # -direction

        # Step 2: push each sigma point through the NONLINEAR function
        # No Jacobian — we just evaluate f at each point directly
        sigma_pred = np.array([f(s) for s in sigma_pts])

        # Step 3: reconstruct predicted mean from weighted sigma points
        x = np.sum(Wm[:, None] * sigma_pred, axis=0)

        # Step 4: reconstruct predicted covariance from weighted deviations
        diff = sigma_pred - x
        P = sum(Wc[i] * np.outer(diff[i], diff[i]) for i in range(2 * n + 1)) + Q

        # ── UKF UPDATE ───────────────────────────────────────────────────────
        # Project sigma points through sensor model (linear H here)
        z_sigma = sigma_pred @ H.T                          # predicted measurements
        z_pred  = np.sum(Wm[:, None] * z_sigma, axis=0)    # predicted mean measurement

        # Innovation covariance
        dz = z_sigma - z_pred
        S  = sum(Wc[i] * np.outer(dz[i], dz[i]) for i in range(2 * n + 1)) + R

        # Cross-covariance between state and measurement
        dx = sigma_pred - x
        Pxz = sum(Wc[i] * np.outer(dx[i], dz[i]) for i in range(2 * n + 1))

        # Kalman gain and update (same form as linear KF)
        K = Pxz @ np.linalg.inv(S)
        x = x + K @ (np.array([z]) - z_pred)
        P = P - K @ S @ K.T

        true_angles.append(x_true[0])
        measurements.append(z)
        estimates.append(x[0])

    return true_angles, measurements, estimates


# ═══════════════════════════════════════════════════════════════════════════════
# PLOT ALL THREE
# ═══════════════════════════════════════════════════════════════════════════════

fig, axes = plt.subplots(3, 1, figsize=(12, 12))

# ── Plot 1: Linear KF ─────────────────────────────────────────────────────────
true_pos, meas, est = linear_kf()
ax = axes[0]
ax.plot(true_pos, label="True position", linewidth=2, color="blue")
ax.plot(meas,     label="Noisy sensor",  linewidth=1, linestyle="dotted", alpha=0.6, color="orange")
ax.plot(est,      label="Linear KF estimate", linewidth=2, color="green")
ax.set_title("Linear KF — 1D constant-velocity tracking\n"
             "Linear dynamics (A matrix) + linear sensor (H matrix)\n"
             "Predict: x=Ax, P=APAᵀ+Q  |  Update: K=PHᵀS⁻¹, x+=K·innov, P=(I-KH)P",
             fontsize=9)
ax.set_ylabel("Position (m)")
ax.legend()
ax.grid(True, alpha=0.3)

# ── Plot 2: EKF ───────────────────────────────────────────────────────────────
true_ang, meas_ekf, est_ekf = ekf()
ax = axes[1]
t = np.arange(len(true_ang)) * 0.05
ax.plot(t, true_ang,  label="True angle",       linewidth=2, color="blue")
ax.plot(t, meas_ekf,  label="Noisy sensor",     linewidth=1, linestyle="dotted", alpha=0.6, color="orange")
ax.plot(t, est_ekf,   label="EKF estimate",     linewidth=2, color="red")
ax.set_title("EKF — Nonlinear pendulum (sin(θ) makes it nonlinear)\n"
             "Key step: linearize f(x) with Jacobian F = df/dx at each timestep\n"
             "Predict: x=f(x), P=FPFᵀ+Q  |  Update: same as linear KF",
             fontsize=9)
ax.set_ylabel("Angle (rad)")
ax.legend()
ax.grid(True, alpha=0.3)

# ── Plot 3: UKF ───────────────────────────────────────────────────────────────
true_ang_u, meas_ukf, est_ukf = ukf()
ax = axes[2]
t = np.arange(len(true_ang_u)) * 0.05
ax.plot(t, true_ang_u, label="True angle",      linewidth=2, color="blue")
ax.plot(t, meas_ukf,   label="Noisy sensor",    linewidth=1, linestyle="dotted", alpha=0.6, color="orange")
ax.plot(t, est_ukf,    label="UKF estimate",    linewidth=2, color="purple")
ax.set_title("UKF — Same pendulum, no Jacobians needed\n"
             "Key step: generate 2n+1 sigma points, push through f(x), reconstruct mean+covariance\n"
             "More accurate than EKF for strong nonlinearity. More compute, but no Jacobian derivation.",
             fontsize=9)
ax.set_xlabel("Time (s)")
ax.set_ylabel("Angle (rad)")
ax.legend()
ax.grid(True, alpha=0.3)

plt.tight_layout()
plt.savefig("/tmp/kalman_variants.png", dpi=150)
plt.show()
print("Plot saved to /tmp/kalman_variants.png")

# ── SUMMARY TABLE ─────────────────────────────────────────────────────────────
print("""
┌─────────────────┬──────────────────────────────┬─────────────────────────────────┐
│  Filter         │  When to use                 │  Key difference                 │
├─────────────────┼──────────────────────────────┼─────────────────────────────────┤
│  Linear KF      │  Linear dynamics + sensor    │  Exact — no approximation       │
│  EKF            │  Nonlinear dynamics/sensor   │  Jacobian linearizes each step  │
│  UKF            │  Strongly nonlinear          │  Sigma points, no Jacobian      │
└─────────────────┴──────────────────────────────┴─────────────────────────────────┘

Interview one-liners:
  KF  → "optimal for linear Gaussian systems — exact solution"
  EKF → "linearizes the nonlinear model at the current estimate via Jacobians — the
          workhorse for real robots (SLAM, pose estimation)"
  UKF → "avoids Jacobians by propagating sigma points through the nonlinear function —
          better accuracy at the cost of more function evaluations"
""")
