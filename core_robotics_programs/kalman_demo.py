import numpy as np
import matplotlib.pyplot as plt

np.random.seed(42)
dt = 0.1
steps = 100

# True state: constant velocity
true_pos = np.array([dt * i for i in range(steps)])

# Noisy measurements (position only, ±2m noise)
measurements = true_pos + np.random.normal(0, 2.0, steps)

# --- Kalman filter ---
A = np.array([[1, dt], [0, 1]])   # constant velocity model
H = np.array([[1, 0]])            # we only measure position
Q = np.eye(2) * 0.01             # small process noise
R = np.array([[4.0]])             # measurement noise (2^2)

x = np.array([0.0, 0.0])        # initial state [pos, vel]
P = np.eye(2)                    # initial uncertainty

estimates = []

for z in measurements:
    # PREDICT
    x = A @ x
    P = A @ P @ A.T + Q

    # UPDATE
    K = P @ H.T @ np.linalg.inv(H @ P @ H.T + R)
    x = x + K @ (np.array([z]) - H @ x)
    P = (np.eye(2) - K @ H) @ P

    estimates.append(x[0])

plt.figure(figsize=(10, 5))
plt.plot(true_pos, label="True position", linewidth=2)
plt.plot(measurements, label="Noisy sensor", alpha=0.5, linestyle="dotted")
plt.plot(estimates, label="Kalman estimate", linewidth=2)
plt.legend()
plt.xlabel("Timestep")
plt.ylabel("Position (m)")
plt.title("Kalman Filter: blending model + noisy sensor")
plt.tight_layout()
plt.savefig("/tmp/kalman_demo.png")
plt.show()
print("Plot saved to /tmp/kalman_demo.png")
