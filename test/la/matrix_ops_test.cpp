#include <la/matrix.hpp>
#include <cassert>
#include <iostream>
#include <cmath>
#include <stdexcept>
#include <string>

void test_matrix_addition() {
    la::dmatrix<int> a = {{1, 2}, {3, 4}};
    la::dmatrix<int> b = {{10, 20}, {30, 40}};
    la::dmatrix<int> c = a + b;
    assert(c(0, 0) == 11);
    assert(c(0, 1) == 22);
    assert(c(1, 0) == 33);
    assert(c(1, 1) == 44);

    // Fixed matrix addition
    la::fmatrix<int, 2, 2> fa = {{1, 2}, {3, 4}};
    la::fmatrix<int, 2, 2> fb = {{5, 6}, {7, 8}};
    la::fmatrix<int, 2, 2> fc = fa + fb;
    assert(fc(0, 0) == 6);
    assert(fc(1, 1) == 12);
}

void test_matrix_subtraction() {
    la::dmatrix<int> a = {{10, 20}, {30, 40}};
    la::dmatrix<int> b = {{1, 2}, {3, 4}};
    la::dmatrix<int> c = a - b;
    assert(c(0, 0) == 9);
    assert(c(0, 1) == 18);
    assert(c(1, 0) == 27);
    assert(c(1, 1) == 36);
}

void test_subtraction_error_message() {
    // Bug #11: error message should say "subtraction" not "addition"
    la::dmatrix<int> a = {{1, 2}, {3, 4}};
    la::dmatrix<int> b = {{1, 2, 3}, {4, 5, 6}};
    try {
        la::dmatrix<int> c = a - b;
        assert(false && "Should have thrown");
    } catch (const std::runtime_error& e) {
        std::string msg = e.what();
        assert(msg.find("subtraction") != std::string::npos);
    }
}

void test_scalar_multiply() {
    la::dmatrix<int> a = {{1, 2}, {3, 4}};

    // scalar * matrix
    la::dmatrix<int> b = 3 * a;
    assert(b(0, 0) == 3);
    assert(b(0, 1) == 6);
    assert(b(1, 0) == 9);
    assert(b(1, 1) == 12);

    // matrix * scalar
    la::dmatrix<int> c = a * 5;
    assert(c(0, 0) == 5);
    assert(c(1, 1) == 20);
}

void test_scalar_divide() {
    la::dmatrix<double> a = {{10.0, 20.0}, {30.0, 40.0}};
    la::dmatrix<double> c = a / 10.0;
    assert(c(0, 0) == 1.0);
    assert(c(0, 1) == 2.0);
    assert(c(1, 0) == 3.0);
    assert(c(1, 1) == 4.0);
}

void test_matrix_multiply() {
    la::dmatrix<int> a = {{1, 2, 3}, {4, 5, 6}};
    la::dmatrix<int> b = {{7, 8}, {9, 10}, {11, 12}};
    la::dmatrix<int> c = a * b;
    assert(c.rows() == 2);
    assert(c.cols() == 2);
    assert(c(0, 0) == 58);
    assert(c(0, 1) == 64);
    assert(c(1, 0) == 139);
    assert(c(1, 1) == 154);
}

void test_transpose() {
    la::dmatrix<int> a = {{1, 2, 3}, {4, 5, 6}};
    la::dmatrix<int> at = la::transpose(a);
    assert(at.rows() == 3);
    assert(at.cols() == 2);
    assert(at(0, 0) == 1);
    assert(at(1, 0) == 2);
    assert(at(2, 0) == 3);
    assert(at(0, 1) == 4);
    assert(at(1, 1) == 5);
    assert(at(2, 1) == 6);
}

void test_norm() {
    la::dmatrix<double> a = {{3.0, 4.0}};
    double n = la::norm<double>(a);
    assert(std::abs(n - 5.0) < 1e-9);

    la::fmatrix<double, 2, 2> b = {{1.0, 2.0}, {3.0, 4.0}};
    double n2 = la::norm<double>(b);
    assert(std::abs(n2 - std::sqrt(30.0)) < 1e-9);
}

void test_expression_chain() {
    la::dmatrix<int> a = {{1, 2}, {3, 4}};
    la::dmatrix<int> b = {{10, 20}, {30, 40}};
    la::dmatrix<int> c = {{100, 200}, {300, 400}};

    la::dmatrix<int> result = a + b + c;
    assert(result(0, 0) == 111);
    assert(result(0, 1) == 222);
    assert(result(1, 0) == 333);
    assert(result(1, 1) == 444);
}

void test_expression_equality() {
    la::dmatrix<int> a = {{1, 2}, {3, 4}};
    la::dmatrix<int> b = {{10, 20}, {30, 40}};
    la::dmatrix<int> expected = {{11, 22}, {33, 44}};

    assert(expected == (a + b));
}

int main() {
    test_matrix_addition();
    test_matrix_subtraction();
    test_subtraction_error_message();
    test_scalar_multiply();
    test_scalar_divide();
    test_matrix_multiply();
    test_transpose();
    test_norm();
    test_expression_chain();
    test_expression_equality();

    std::cout << "All tests passed (matrix_ops_test)" << std::endl;
    return 0;
}
