#include <eigen3/Eigen/Dense>
#include <iostream>
#include <random>

using namespace Eigen;

void predict(Vector2d& x, Matrix2d& P, const Matrix2d& A, const Matrix2d& Q) {
    x = A * x;
    P = A * P * A.transpose() + Q;
}

void update(Vector2d& x, Matrix2d& P, double z, const Matrix<double,1,2>& H, double R) {
    double S = (H * P * H.transpose())(0, 0) + R;
    Matrix<double,2,1> K = P * H.transpose() / S;
    x = x + K * (z - (H * x)(0));
    P = (Matrix2d::Identity() - K * H) * P;
}

int main() {
    double dt = 0.1;

    Matrix2d A;
    A << 1, dt,
         0,  1;

    Matrix<double,1,2> H;
    H << 1, 0;

    Matrix2d Q = Matrix2d::Identity() * 0.01;
    double R = 4.0;

    Vector2d x;
    x << 0, 0;

    Matrix2d P = Matrix2d::Identity();

    std::default_random_engine gen(42);
    std::normal_distribution<double> noise(0.0, 2.0);

    for (int i = 0; i < 100; i++) {
        double true_pos = dt * i;
        double z = true_pos + noise(gen);

        predict(x, P, A, Q);
        update(x, P, z, H, R);

        std::cout << "step " << i
                  << "  true: " << true_pos
                  << "  measured: " << z
                  << "  estimate: " << x(0) << "\n";
    }
}
