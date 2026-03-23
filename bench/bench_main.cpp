#include <la/matrix.hpp>
#include <la/vector.hpp>
#include <chrono>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <string>

// Prevent the compiler from optimizing away a value
template<typename T>
void do_not_optimize(T const& val) {
    asm volatile("" : : "r,m"(val) : "memory");
}

// Simple timer using high_resolution_clock
struct Timer {
    using clock = std::chrono::high_resolution_clock;
    clock::time_point start;
    Timer() : start(clock::now()) {}
    double elapsed_us() const {
        auto end = clock::now();
        return std::chrono::duration<double, std::micro>(end - start).count();
    }
};

// Returns median time in microseconds over N runs
template<typename F>
double bench(F&& f, int runs = 7) {
    // Warmup
    f();
    f();

    std::vector<double> times;
    times.reserve(runs);
    for (int i = 0; i < runs; i++) {
        Timer t;
        f();
        times.push_back(t.elapsed_us());
    }
    std::sort(times.begin(), times.end());
    return times[runs / 2];
}

void print_result(const std::string& name, double us) {
    if (us > 1000.0) {
        std::cout << "  " << name << ": " << (us / 1000.0) << " ms" << std::endl;
    } else {
        std::cout << "  " << name << ": " << us << " us" << std::endl;
    }
}

// ============================================================
// Benchmark 1: Matrix multiply loop order (the big one)
// ============================================================
void bench_matmul() {
    std::cout << "=== Matrix Multiply (loop order) ===" << std::endl;

    for (int N : {32, 64, 128, 256}) {
        la::dmatrix<double> a(N, N);
        la::dmatrix<double> b(N, N);
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                a(i, j) = i * 0.01 + j * 0.001;
                b(i, j) = j * 0.01 - i * 0.001;
            }
        }

        double us = bench([&]() {
            auto c = a * b;
            do_not_optimize(c(0, 0));
        }, 5);
        print_result("matmul " + std::to_string(N) + "x" + std::to_string(N), us);
    }
    std::cout << std::endl;
}

// ============================================================
// Benchmark 2: Fixed vs dynamic matmul (heap allocation cost)
// ============================================================
void bench_fixed_vs_dynamic_matmul() {
    std::cout << "=== Fixed vs Dynamic 4x4 Matmul ===" << std::endl;

    la::fmatrix<double, 4, 4> fa = {{1.0, 2.0, 3.0, 4.0}, {5.0, 6.0, 7.0, 8.0}, {9.0, 10.0, 11.0, 12.0}, {13.0, 14.0, 15.0, 16.0}};
    la::fmatrix<double, 4, 4> fb = {{16.0, 15.0, 14.0, 13.0}, {12.0, 11.0, 10.0, 9.0}, {8.0, 7.0, 6.0, 5.0}, {4.0, 3.0, 2.0, 1.0}};

    la::dmatrix<double> da = {{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}, {13, 14, 15, 16}};
    la::dmatrix<double> db = {{16, 15, 14, 13}, {12, 11, 10, 9}, {8, 7, 6, 5}, {4, 3, 2, 1}};

    constexpr int ITERS = 1000;

    double us_fixed = bench([&]() {
        for (int i = 0; i < ITERS; i++) {
            auto c = fa * fb;
            do_not_optimize(c(0, 0));
        }
    });
    print_result("fixed  4x4 x" + std::to_string(ITERS), us_fixed);

    double us_dynamic = bench([&]() {
        for (int i = 0; i < ITERS; i++) {
            auto c = da * db;
            do_not_optimize(c(0, 0));
        }
    });
    print_result("dynamic 4x4 x" + std::to_string(ITERS), us_dynamic);
    std::cout << std::endl;
}

// ============================================================
// Benchmark 3: norm() vs squared_norm() (sqrt overhead)
// ============================================================
void bench_norm() {
    std::cout << "=== Norm vs Squared Norm ===" << std::endl;

    constexpr int N = 10000;
    la::dvector<double> v(N);
    for (int i = 0; i < N; i++) {
        v[i] = i * 0.001;
    }

    constexpr int ITERS = 1000;

    double us_norm = bench([&]() {
        for (int i = 0; i < ITERS; i++) {
            double n = la::norm<double>(v);
            do_not_optimize(n);
        }
    });
    print_result("norm x" + std::to_string(ITERS), us_norm);

    double us_squared = bench([&]() {
        for (int i = 0; i < ITERS; i++) {
            double res = la::squared_norm<double>(v);
            do_not_optimize(res);
        }
    });
    print_result("squared_norm x" + std::to_string(ITERS), us_squared);
    std::cout << std::endl;
}

// ============================================================
// Benchmark 4: zeros() double-init overhead
// ============================================================
void bench_zeros() {
    std::cout << "=== Zeros Construction ===" << std::endl;

    constexpr int ITERS = 10000;

    // Dynamic zeros — single init
    double us_dyn = bench([&]() {
        for (int i = 0; i < ITERS; i++) {
            auto m = la::zeros<double>(64, 64);
            do_not_optimize(m(0, 0));
        }
    });
    print_result("dynamic zeros 64x64 x" + std::to_string(ITERS), us_dyn);

    // Fixed vector construction — test value-init overhead
    double us_fixed = bench([&]() {
        for (int i = 0; i < ITERS; i++) {
            la::fvector<double, 64> v = {
                0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
                0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
                0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
                0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0
            };
            do_not_optimize(v[0]);
        }
    });
    print_result("fixed vector<64> init x" + std::to_string(ITERS), us_fixed);
    std::cout << std::endl;
}

// ============================================================
// Benchmark 5: Expression template chain (a + b + c + d)
// ============================================================
void bench_expression_chain() {
    std::cout << "=== Expression Chain (a + b + c + d) ===" << std::endl;

    constexpr int N = 1000;
    la::dvector<double> a(N), b(N), c(N), d(N);
    for (int i = 0; i < N; i++) {
        a[i] = i * 1.0;
        b[i] = i * 2.0;
        c[i] = i * 3.0;
        d[i] = i * 4.0;
    }

    constexpr int ITERS = 10000;

    double us = bench([&]() {
        for (int i = 0; i < ITERS; i++) {
            la::dvector<double> result = a + b + c + d;
            do_not_optimize(result[0]);
        }
    });
    print_result("vec add chain (1000 elems) x" + std::to_string(ITERS), us);

    // Matrix version
    la::dmatrix<double> ma = {{1, 2, 3}, {4, 5, 6}};
    la::dmatrix<double> mb = {{10, 20, 30}, {40, 50, 60}};
    la::dmatrix<double> mc = {{100, 200, 300}, {400, 500, 600}};

    double us_mat = bench([&]() {
        for (int i = 0; i < ITERS; i++) {
            la::dmatrix<double> result = ma + mb + mc;
            do_not_optimize(result(0, 0));
        }
    });
    print_result("mat add chain (2x3) x" + std::to_string(ITERS), us_mat);
    std::cout << std::endl;
}

// ============================================================
// Benchmark 6: Vector inner product
// ============================================================
void bench_inner_product() {
    std::cout << "=== Inner Product ===" << std::endl;

    constexpr int N = 10000;
    la::dvector<double> a(N), b(N);
    for (int i = 0; i < N; i++) {
        a[i] = i * 0.001;
        b[i] = (N - i) * 0.001;
    }

    constexpr int ITERS = 10000;

    double us = bench([&]() {
        for (int i = 0; i < ITERS; i++) {
            auto r = la::inner_product(a, b);
            do_not_optimize(r);
        }
    });
    print_result("inner_product (10000 elems) x" + std::to_string(ITERS), us);
    std::cout << std::endl;
}

// ============================================================
// Benchmark 7: Matrix transpose
// ============================================================
void bench_transpose() {
    std::cout << "=== Transpose ===" << std::endl;

    la::dmatrix<double> m(128, 128);
    for (int i = 0; i < 128; i++)
        for (int j = 0; j < 128; j++)
            m(i, j) = i + j * 0.01;

    constexpr int ITERS = 1000;

    double us = bench([&]() {
        for (int i = 0; i < ITERS; i++) {
            la::dmatrix<double> t = la::transpose(m);
            do_not_optimize(t(0, 0));
        }
    });
    print_result("transpose 128x128 x" + std::to_string(ITERS), us);
    std::cout << std::endl;
}

int main() {
    std::cout << "=====================================" << std::endl;
    std::cout << "  la library benchmarks" << std::endl;
    std::cout << "=====================================" << std::endl;
    std::cout << std::endl;

    bench_matmul();
    bench_fixed_vs_dynamic_matmul();
    bench_norm();
    bench_zeros();
    bench_expression_chain();
    bench_inner_product();
    bench_transpose();

    std::cout << "=====================================" << std::endl;
    std::cout << "  Done." << std::endl;
    std::cout << "=====================================" << std::endl;
    return 0;
}
