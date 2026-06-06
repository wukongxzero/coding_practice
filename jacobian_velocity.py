import numpy as np

def get_jacobian(q, L1=1.0, L2=1.0):
    q1, q2 = q[0], q[1]
    dx_dq1 = -L1 * np.sin(q1) - L2 * np.sin(q1 + q2)
    dx_dq2 = -L2 * np.sin(q1 + q2)
    dy_dq1 =  L1 * np.cos(q1) + L2 * np.cos(q1 + q2)
    dy_dq2 =  L2 * np.cos(q1 + q2)
    return np.array([[dx_dq1, dx_dq2],
                     [dy_dq1, dy_dq2]])

def compute_end_effector_velocity(q, q_dot):
    return get_jacobian(q) @ q_dot

if __name__ == "__main__":
    q = np.array([0.5, 0.5])
    q_dot = np.array([1.0, 0.5])
    print("EE Velocity:", compute_end_effector_velocity(q, q_dot))
