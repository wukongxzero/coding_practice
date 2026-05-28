import numpy as np

L = [1.0, 2.0, 3.0]
q = np.array([np.pi/6, np.pi/6, np.pi/6])

def fk(q, L):
    a1 = q[0]
    a2 = q[0] + q[1]
    a3 = q[0] + q[1] + q[2]
    x = L[0]*np.cos(a1) + L[1]*np.cos(a2) + L[2]*np.cos(a3)
    y = L[0]*np.sin(a1) + L[1]*np.sin(a2) + L[2]*np.sin(a3)
    print(f"EE position: x={x:.4f}, y={y:.4f}")
    return np.array([x, y])

def jacobian(q, L):
    a1 = q[0]
    a2 = q[0] + q[1]
    a3 = q[0] + q[1] + q[2]
    J = np.array([
        [-L[0]*np.sin(a1) - L[1]*np.sin(a2) - L[2]*np.sin(a3),
         -L[1]*np.sin(a2) - L[2]*np.sin(a3),
         -L[2]*np.sin(a3)],
        [ L[0]*np.cos(a1) + L[1]*np.cos(a2) + L[2]*np.cos(a3),
          L[1]*np.cos(a2) + L[2]*np.cos(a3),
          L[2]*np.cos(a3)]
    ])
    print(f"J =\n{J}")
    return J

def dlsik(J, xdot_d, lam=0.01):
    m = J.shape[0]                      # task-space dim (here 2), not hardcoded
    return J.T @ np.linalg.inv(J @ J.T + lam**2 * np.eye(m)) @ xdot_d

ee = fk(q, L)
J = jacobian(q, L)
q_dot = dlsik(J, np.array([0.1, 0.2]))
print("q_dot =", q_dot)
print("J @ q_dot =", J @ q_dot)         # should ≈ [0.1, 0.2]