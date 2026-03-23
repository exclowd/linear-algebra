#include <la/matrix.hpp>
#include <la/vector.hpp>
#include <cassert>
#include <iostream>
#include <cmath>
#include <type_traits>

void test_fixed_matrix_default_ctor() {
    la::fmatrix<int, 2, 3> m;
    assert(m.rows() == 2);
    assert(m.cols() == 3);
    // Value-initialized: all zeros
    for (std::size_t i = 0; i < m.rows(); i++)
        for (std::size_t j = 0; j < m.cols(); j++)
            assert(m(i, j) == 0);
}

void test_fixed_vector_default_ctor() {
    la::fvector<int, 5> v;
    assert(v.size() == 5);
    for (std::size_t i = 0; i < v.size(); i++)
        assert(v[i] == 0);
}

void test_fixed_matmul_returns_fixed() {
    la::fmatrix<int, 2, 3> a = {{1, 2, 3}, {4, 5, 6}};
    la::fmatrix<int, 3, 2> b = {{7, 8}, {9, 10}, {11, 12}};
    auto c = a * b;

    // Result should be fmatrix<int, 2, 2> (stack-allocated)
    static_assert(std::is_same_v<decltype(c), la::fmatrix<int, 2, 2>>,
                  "Fixed matmul should return fmatrix");

    assert(c.rows() == 2);
    assert(c.cols() == 2);
    assert(c(0, 0) == 58);
    assert(c(0, 1) == 64);
    assert(c(1, 0) == 139);
    assert(c(1, 1) == 154);
}

void test_squared_norm_vector() {
    la::dvector<double> v = {3.0, 4.0};
    double sn = la::squared_norm<double>(v);
    assert(std::abs(sn - 25.0) < 1e-9);

    double n = la::norm<double>(v);
    assert(std::abs(n - 5.0) < 1e-9);
    assert(std::abs(sn - n * n) < 1e-9);
}

void test_squared_norm_matrix() {
    la::dmatrix<double> m = {{1.0, 2.0}, {3.0, 4.0}};
    double sn = la::squared_norm<double>(m);
    assert(std::abs(sn - 30.0) < 1e-9);

    double n = la::norm<double>(m);
    assert(std::abs(sn - n * n) < 1e-9);
}

void test_data_pointer_vector() {
    la::dvector<int> v = {10, 20, 30};
    int* p = v.begin().operator->();  // iterator-based
    assert(p != nullptr);

    la::fvector<int, 3> fv = {1, 2, 3};
    assert(fv[0] == 1);
}

void test_matrix_linear_access() {
    la::fmatrix<int, 2, 2> m = {{1, 2}, {3, 4}};
    int sum = 0;
    for (std::size_t i = 0; i < m.data_size(); i++)
        sum += m[i];
    assert(sum == 10);

    la::dmatrix<int> dm = {{5, 6}, {7, 8}};
    sum = 0;
    for (std::size_t i = 0; i < dm.data_size(); i++)
        sum += dm[i];
    assert(sum == 26);
}

void test_expression_chain_no_dangling() {
    // This tests that a + b + c doesn't create dangling references
    // The inner (a + b) expression is stored by value in the outer expression
    la::dvector<int> a = {1, 2, 3};
    la::dvector<int> b = {10, 20, 30};
    la::dvector<int> c = {100, 200, 300};

    la::dvector<int> result = a + b + c;
    assert(result[0] == 111);
    assert(result[1] == 222);
    assert(result[2] == 333);

    // Matrix version
    la::dmatrix<int> ma = {{1, 2}, {3, 4}};
    la::dmatrix<int> mb = {{10, 20}, {30, 40}};
    la::dmatrix<int> mc = {{100, 200}, {300, 400}};

    la::dmatrix<int> mresult = ma + mb + mc;
    assert(mresult(0, 0) == 111);
    assert(mresult(1, 1) == 444);
}

void test_zeros_fixed_now_works() {
    // With the default constructor, zeros<T,R,C>() now works
    auto m = la::zeros<int, 2, 3>();
    assert(m.rows() == 2);
    assert(m.cols() == 3);
    for (std::size_t i = 0; i < 2; i++)
        for (std::size_t j = 0; j < 3; j++)
            assert(m(i, j) == 0);

    auto v = la::zeros<int, 5>();
    assert(v.size() == 5);
    for (std::size_t i = 0; i < 5; i++)
        assert(v[i] == 0);
}

void test_slice_noexcept() {
    la::slice<std::size_t> a(1, 4);
    la::slice<std::size_t> b(1, 4);

    // These should be noexcept
    static_assert(noexcept(a == b));
    static_assert(noexcept(a != b));
    static_assert(noexcept(a.start()));
    static_assert(noexcept(a.end()));
    static_assert(noexcept(a.size()));
}

int main() {
    test_fixed_matrix_default_ctor();
    test_fixed_vector_default_ctor();
    test_fixed_matmul_returns_fixed();
    test_squared_norm_vector();
    test_squared_norm_matrix();
    test_data_pointer_vector();
    test_matrix_linear_access();
    test_expression_chain_no_dangling();
    test_zeros_fixed_now_works();
    test_slice_noexcept();

    std::cout << "All tests passed (optimization_test)" << std::endl;
    return 0;
}
