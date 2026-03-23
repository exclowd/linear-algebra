#include <la/vector.hpp>
#include <cassert>
#include <iostream>
#include <cmath>
#include <stdexcept>
#include <string>

void test_fixed_vector_construction() {
    la::fvector<int, 4> v = {1, 2, 3, 4};
    assert(v.size() == 4);
    assert(v[0] == 1);
    assert(v[1] == 2);
    assert(v[2] == 3);
    assert(v[3] == 4);

    la::fvector<int, 3> v2 = {10, 20, 30};
    assert(v2.size() == 3);
    assert(!v2.empty());
}

void test_dynamic_vector_construction() {
    la::dvector<int> v(5);
    assert(v.size() == 5);
    for (std::size_t i = 0; i < v.size(); i++) {
        assert(v[i] == 0);
    }

    la::dvector<int> v2 = {1, 2, 3};
    assert(v2.size() == 3);
    assert(v2[0] == 1);
    assert(v2[2] == 3);

    la::dvector<int> v3(0);
    assert(v3.empty());
    assert(v3.size() == 0);
}

void test_vector_access() {
    la::fvector<int, 3> v = {10, 20, 30};
    assert(v[0] == 10);
    assert(v[1] == 20);
    assert(v[2] == 30);

    // Mutation
    v[1] = 99;
    assert(v[1] == 99);

    // Const access
    const la::fvector<int, 3> cv = {1, 2, 3};
    assert(cv[0] == 1);
}

void test_cross_engine_vector_copy() {
    // Fixed -> Dynamic
    la::fvector<int, 3> fv = {1, 2, 3};
    la::dvector<int> dv(fv);
    assert(dv.size() == 3);
    assert(dv[0] == 1);
    assert(dv[2] == 3);

    // Dynamic -> Fixed
    la::dvector<int> dv2 = {10, 20, 30};
    la::fvector<int, 3> fv2(dv2);
    assert(fv2[0] == 10);
    assert(fv2[2] == 30);

    // Cross-engine assignment
    la::dvector<int> dv3(3);
    dv3 = fv;
    assert(dv3[0] == 1);
    assert(dv3[2] == 3);
}

void test_subvector_slicing() {
    la::dvector<int> v = {10, 20, 30, 40, 50};
    using slice = la::slice<std::size_t>;

    auto sub = v(slice(1, 4));
    assert(sub.size() == 3);
    assert(sub[0] == 20);
    assert(sub[1] == 30);
    assert(sub[2] == 40);

    // Full range
    auto full = v(slice(0, 5));
    assert(full.size() == 5);
    assert(full[0] == 10);
    assert(full[4] == 50);

    // Single element
    auto single = v(slice(2, 3));
    assert(single.size() == 1);
    assert(single[0] == 30);
}

void test_mutable_subvector() {
    la::dvector<int> v = {1, 2, 3, 4, 5};
    using slice = la::slice<std::size_t>;

    auto sub = v(slice(1, 4));
    sub[0] = 99;
    assert(v[1] == 99);

    sub[2] = 88;
    assert(v[3] == 88);
}

void test_vector_addition() {
    la::dvector<int> a = {1, 2, 3};
    la::dvector<int> b = {10, 20, 30};
    la::dvector<int> c = a + b;
    assert(c[0] == 11);
    assert(c[1] == 22);
    assert(c[2] == 33);
}

void test_vector_subtraction() {
    la::dvector<int> a = {10, 20, 30};
    la::dvector<int> b = {1, 2, 3};
    la::dvector<int> c = a - b;
    assert(c[0] == 9);
    assert(c[1] == 18);
    assert(c[2] == 27);

    // Error message check
    la::dvector<int> d = {1, 2};
    try {
        la::dvector<int> e = a - d;
        assert(false && "Should have thrown");
    } catch (const std::runtime_error& e) {
        std::string msg = e.what();
        assert(msg.find("subtraction") != std::string::npos);
    }
}

void test_vector_scalar_ops() {
    la::dvector<int> a = {2, 4, 6};

    // scalar * vector
    la::dvector<int> b = 3 * a;
    assert(b[0] == 6);
    assert(b[1] == 12);
    assert(b[2] == 18);

    // vector * scalar
    la::dvector<int> c = a * 5;
    assert(c[0] == 10);
    assert(c[1] == 20);
    assert(c[2] == 30);

    // vector / scalar
    la::dvector<double> d = {10.0, 20.0, 30.0};
    la::dvector<double> e = d / 10.0;
    assert(e[0] == 1.0);
    assert(e[1] == 2.0);
    assert(e[2] == 3.0);
}

void test_vector_elementwise_multiply() {
    la::dvector<int> a = {2, 3, 4};
    la::dvector<int> b = {5, 6, 7};
    la::dvector<int> c = a * b;
    assert(c[0] == 10);
    assert(c[1] == 18);
    assert(c[2] == 28);

    la::fvector<int, 3> fi = {1, 2, 3};
    la::fvector<int, 3> fi2 = {4, 5, 6};
    la::fvector<int, 3> fi3 = fi * fi2;
    assert(fi3[0] == 4);
    assert(fi3[1] == 10);
    assert(fi3[2] == 18);
}

void test_inner_product() {
    la::dvector<int> a = {1, 2, 3};
    la::dvector<int> b = {4, 5, 6};
    auto result = la::inner_product(a, b);
    assert(result == 32);

    la::fvector<double, 3> fa = {1.0, 0.0, 0.0};
    la::fvector<double, 3> fb = {0.0, 1.0, 0.0};
    auto dot = la::inner_product(fa, fb);
    assert(dot == 0.0);
}

void test_vector_norm() {
    la::dvector<double> v = {3.0, 4.0};
    double n = la::norm<double>(v);
    assert(std::abs(n - 5.0) < 1e-9);

    la::fvector<double, 3> fv = {1.0, 2.0, 2.0};
    double n2 = la::norm<double>(fv);
    assert(std::abs(n2 - 3.0) < 1e-9);
}

void test_vector_view_empty() {
    la::dvector<int> v = {1, 2, 3};
    using slice = la::slice<std::size_t>;

    auto sub = v(slice(1, 3));
    assert(!sub.empty());

    auto empty_sub = v(slice(2, 2));
    assert(empty_sub.empty());
    assert(empty_sub.size() == 0);
}

void test_vector_view_assignment() {
    la::dvector<int> v = {1, 2, 3, 4, 5};
    la::dvector<int> src = {99, 88, 77};
    using slice = la::slice<std::size_t>;

    auto sub = v(slice(1, 4));
    sub = src;
    assert(v[0] == 1);
    assert(v[1] == 99);
    assert(v[2] == 88);
    assert(v[3] == 77);
    assert(v[4] == 5);
}

void test_vector_equality() {
    la::dvector<int> a = {1, 2, 3};
    la::dvector<int> b = {1, 2, 3};
    la::dvector<int> c = {1, 2, 4};

    assert(a == b);
    assert(a != c);

    // Different sizes
    la::dvector<int> d = {1, 2};
    assert(a != d);

    // Cross-engine
    la::fvector<int, 3> fa = {1, 2, 3};
    assert(a == fa);
}

void test_vector_zeros() {
    auto dv = la::zeros<int>(5);
    assert(dv.size() == 5);
    for (std::size_t i = 0; i < 5; i++)
        assert(dv[i] == 0);
}

int main() {
    test_fixed_vector_construction();
    test_dynamic_vector_construction();
    test_vector_access();
    test_cross_engine_vector_copy();
    test_subvector_slicing();
    test_mutable_subvector();
    test_vector_addition();
    test_vector_subtraction();
    test_vector_scalar_ops();
    test_vector_elementwise_multiply();
    test_inner_product();
    test_vector_norm();
    test_vector_view_empty();
    test_vector_view_assignment();
    test_vector_equality();
    test_vector_zeros();

    std::cout << "All tests passed (vector_test)" << std::endl;
    return 0;
}
