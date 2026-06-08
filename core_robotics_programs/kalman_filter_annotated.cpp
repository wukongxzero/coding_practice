#include <eigen3/Eigen/Dense>  // Eigen: matrix math library. Dense = non-sparse matrices
#include <iostream>             // std::cout for printing
#include <random>               // std::normal_distribution for Gaussian noise

using namespace Eigen;          // write Vector2d instead of Eigen::Vector2d

// ─── PREDICT ────────────────────────────────────────────────────────────────
// Called every timestep BEFORE the sensor fires.
// Pushes the estimate forward using the motion model (physics).
// x and P are passed by reference — modified in place, nothing returned.

void predict(Vector2d& x,        // state = [position, velocity], modified in place
             Matrix2d& P,        // 2x2 covariance (how uncertain we are), modified in place
             const Matrix2d& A,  // dynamics matrix: encodes the motion model
             const Matrix2d& Q)  // process noise: how much we distrust the model each step
{
    x = A * x;
    // x̂⁻ = A·x̂
    // Multiplying by A applies one step of physics:
    //   new_position = 1*position + dt*velocity   (row 0 of A)
    //   new_velocity = 0*position + 1*velocity    (row 1 of A — constant velocity)

    P = A * P * A.transpose() + Q;
    // P⁻ = A·P·Aᵀ + Q
    // A·P·Aᵀ: rotates the uncertainty ellipse to match the new state space
    // +Q:     uncertainty GROWS — our model isn't perfect, so we get less sure each step
}

// ─── UPDATE ─────────────────────────────────────────────────────────────────
// Called when a sensor measurement arrives.
// Corrects the prediction by blending model vs sensor, weighted by their uncertainties.

void update(Vector2d& x,               // state estimate — will be corrected toward measurement
            Matrix2d& P,               // covariance — will SHRINK after update (sensor = info)
            double z,                  // raw sensor reading (noisy position)
            const Matrix<double,1,2>& H,  // observation matrix: maps state space → sensor space
                                          // H=[1,0] means: extract position from [pos, vel]
            double R)                  // measurement noise variance (σ² of the sensor)
{
    double S = (H * P * H.transpose())(0, 0) + R;
    // S = innovation covariance — total expected variance of (measurement - prediction)
    // H·P·Hᵀ: predicted uncertainty projected into sensor space (how wrong we expect to be)
    // +R:     add sensor noise on top of that
    // (0,0):  H·P·Hᵀ is a 1×1 Eigen matrix; (0,0) extracts it as a plain double

    Matrix<double,2,1> K = P * H.transpose() / S;
    // K = Kalman gain — the key quantity. Tells us how much to trust the sensor.
    // P·Hᵀ:  covariance projected into sensor direction (2×1 vector)
    // ÷S:    normalize: large S (uncertain/noisy) → small K → ignore sensor, trust model
    //                   small S (confident) → large K → follow the sensor
    // K has shape 2×1: one gain for position state, one for velocity state

    x = x + K * (z - (H * x)(0));
    // Innovation = z - H·x = (what sensor says) - (what we predicted)
    // If they agree → innovation ≈ 0 → no correction
    // If they disagree → innovation is large → K scales how much we shift x
    // (H * x)(0): H·x is a 1×1 Eigen matrix; (0) extracts scalar

    P = (Matrix2d::Identity() - K * H) * P;
    // P shrinks: (I - K·H) is always less than I
    // The measurement gave us information — we became more certain
    // If K is large (we trusted the sensor a lot), P shrinks more
}

int main() {
    double dt = 0.1;  // 100ms per timestep — robot updates at 10 Hz

    // ── MOTION MODEL ────────────────────────────────────────────────────────
    Matrix2d A;
    A << 1, dt,   // new_pos = pos + vel*dt
         0,  1;   // new_vel = vel  (constant velocity assumption)

    // ── OBSERVATION MODEL ────────────────────────────────────────────────────
    Matrix<double,1,2> H;
    H << 1, 0;    // we measure position only (not velocity)
                  // H * [pos, vel]ᵀ = pos

    // ── NOISE TUNING ─────────────────────────────────────────────────────────
    Matrix2d Q = Matrix2d::Identity() * 0.01;  // small: we trust the constant-velocity model
    double R = 4.0;  // sensor std=2m → variance=4. Large R = trust model more than sensor

    // ── INITIAL BELIEF ───────────────────────────────────────────────────────
    Vector2d x;
    x << 0, 0;                          // start: position=0, velocity=0 (guesses)
    Matrix2d P = Matrix2d::Identity();  // moderate initial uncertainty in both states

    // ── NOISE GENERATOR ──────────────────────────────────────────────────────
    std::default_random_engine gen(42);           // seed=42 for reproducibility
    std::normal_distribution<double> noise(0.0, 2.0);  // Gaussian: mean=0, std=2m

    // ── MAIN LOOP ────────────────────────────────────────────────────────────
    for (int i = 0; i < 100; i++) {
        double true_pos = dt * i;          // ground truth: robot at 1 m/s
        double z = true_pos + noise(gen);  // noisy sensor: truth + random Gaussian error

        predict(x, P, A, Q);   // 1. push state forward with motion model
        update(x, P, z, H, R); // 2. correct with the sensor reading

        // x(0) = estimated position
        // x(1) = estimated velocity (we never directly measured this — filter infers it)
        std::cout << "step " << i
                  << "  true: "     << true_pos
                  << "  measured: " << z
                  << "  estimate: " << x(0)
                  << "  est_vel: "  << x(1) << "\n";
    }
}
