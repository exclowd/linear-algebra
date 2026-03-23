#include <la/matrix.hpp>
#include <cassert>
#include <iostream>

void test_fixed_matrix_construction() {
    // Init-list construction
    la::fmatrix<int, 2, 3> m = {{1, 2, 3}, {4, 5, 6}};
    assert(m.rows() == 2);
    assert(m.cols() == 3);
    assert(m(0, 0) == 1);
    assert(m(0, 2) == 3);
    assert(m(1, 0) == 4);
    assert(m(1, 2) == 6);
}

void test_dynamic_matrix_construction() {
    // Size construction
    la::dmatrix<int> m(3, 4);
    assert(m.rows() == 3);
    assert(m.cols() == 4);
    for (std::size_t i = 0; i < m.rows(); i++) {
        for (std::size_t j = 0; j < m.cols(); j++) {
            assert(m(i, j) == 0);
        }
    }

    // Init-list construction
    la::dmatrix<int> m2 = {{1, 2}, {3, 4}};
    assert(m2.rows() == 2);
    assert(m2.cols() == 2);
    assert(m2(0, 0) == 1);
    assert(m2(1, 1) == 4);

    // size_tuple construction
    la::dmatrix<int> m3(std::pair<std::size_t, std::size_t>(2, 5));
    assert(m3.rows() == 2);
    assert(m3.cols() == 5);
}

void test_element_access() {
    la::fmatrix<int, 2, 3> m = {{1, 2, 3}, {4, 5, 6}};

    // operator()(r, c)
    assert(m(0, 0) == 1);
    assert(m(1, 2) == 6);

    // operator()(size_tuple)
    assert(m(std::make_pair<std::size_t, std::size_t>(0, 1)) == 2);

    // operator[] (linear index, row-major)
    assert(m[0] == 1);
    assert(m[1] == 2);
    assert(m[3] == 4);
    assert(m[5] == 6);

    // Mutation via operator()
    m(0, 0) = 99;
    assert(m(0, 0) == 99);

    // Mutation via operator[]
    m[1] = 77;
    assert(m(0, 1) == 77);
}

void test_size_queries() {
    la::fmatrix<int, 3, 4> m = {{1,2,3,4},{5,6,7,8},{9,10,11,12}};
    assert(m.rows() == 3);
    assert(m.cols() == 4);
    assert(m.data_size() == 12);
    auto [r, c] = m.size();
    assert(r == 3);
    assert(c == 4);

    la::dmatrix<int> dm(0, 0);
    assert(dm.empty());
}

void test_cross_engine_copy() {
    // Fixed -> Dynamic
    la::fmatrix<int, 2, 2> fm = {{1, 2}, {3, 4}};
    la::dmatrix<int> dm(fm);
    assert(dm.rows() == 2);
    assert(dm.cols() == 2);
    assert(dm(0, 0) == 1);
    assert(dm(1, 1) == 4);

    // Dynamic -> Fixed
    la::dmatrix<int> dm2 = {{10, 20}, {30, 40}};
    la::fmatrix<int, 2, 2> fm2(dm2);
    assert(fm2(0, 0) == 10);
    assert(fm2(1, 1) == 40);

    // Cross-engine assignment
    la::dmatrix<int> dm3(2, 2);
    dm3 = fm;
    assert(dm3(0, 0) == 1);
    assert(dm3(1, 1) == 4);
}

void test_submatrix_slicing() {
    la::dmatrix<int> m = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    using slice = la::slice<std::size_t>;

    // Row slice: m(1, slice(0,3)) -> row 1
    auto row = m(static_cast<std::size_t>(1), slice(0, 3));
    assert(row.rows() == 1);
    assert(row.cols() == 3);
    assert(row(0, 0) == 4);
    assert(row(0, 1) == 5);
    assert(row(0, 2) == 6);

    // Col slice: m(slice(0,3), 2) -> col 2
    auto col = m(slice(0, 3), static_cast<std::size_t>(2));
    assert(col.rows() == 3);
    assert(col.cols() == 1);
    assert(col(0, 0) == 3);
    assert(col(1, 0) == 6);
    assert(col(2, 0) == 9);

    // Submatrix: m(slice(0,2), slice(1,3))
    auto sub = m(slice(0, 2), slice(1, 3));
    assert(sub.rows() == 2);
    assert(sub.cols() == 2);
    assert(sub(0, 0) == 2);
    assert(sub(0, 1) == 3);
    assert(sub(1, 0) == 5);
    assert(sub(1, 1) == 6);
}

void test_matrix_view_operator_bracket() {
    // Bug #5: matrix_view operator[] row/col ordering
    la::dmatrix<int> m = {{1, 2, 3}, {4, 5, 6}};
    using slice = la::slice<std::size_t>;

    auto sub = m(slice(0, 2), slice(0, 3));
    // operator[] should use row-major: idx / cols, idx % cols
    assert(sub[0] == 1);  // (0,0)
    assert(sub[1] == 2);  // (0,1)
    assert(sub[2] == 3);  // (0,2)
    assert(sub[3] == 4);  // (1,0)
    assert(sub[4] == 5);  // (1,1)
    assert(sub[5] == 6);  // (1,2)
}

void test_matrix_view_empty() {
    // Bug #3: matrix_view empty() correctness
    la::dmatrix<int> m = {{1, 2}, {3, 4}};
    using slice = la::slice<std::size_t>;

    auto sub = m(slice(0, 2), slice(0, 2));
    assert(!sub.empty());

    // A view with zero rows or cols should be empty
    auto empty_sub = m(slice(0, 0), slice(0, 2));
    assert(empty_sub.empty());
}

void test_matrix_view_size_tuple() {
    // Bug #2: matrix_view size_tuple type uses engine's size_tuple (std::pair)
    la::dmatrix<int> m = {{1, 2, 3}, {4, 5, 6}};
    using slice = la::slice<std::size_t>;

    auto sub = m(slice(0, 2), slice(1, 3));
    auto sz = sub.size();
    assert(sz.first == 2);
    assert(sz.second == 2);
}

void test_mutable_submatrix() {
    la::dmatrix<int> m = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    using slice = la::slice<std::size_t>;

    auto sub = m(slice(0, 2), slice(0, 2));
    sub(0, 0) = 99;
    assert(m(0, 0) == 99);

    sub(1, 1) = 88;
    assert(m(1, 1) == 88);
}

void test_zeros() {
    auto dm = la::zeros<int>(3, 4);
    assert(dm.rows() == 3);
    assert(dm.cols() == 4);
    for (std::size_t i = 0; i < 3; i++)
        for (std::size_t j = 0; j < 4; j++)
            assert(dm(i, j) == 0);

    // Note: zeros<T,R,C>() for fixed matrices requires a default constructor
    // which matrix doesn't provide, so we only test the dynamic version.
}

void test_equality_inequality() {
    la::dmatrix<int> a = {{1, 2}, {3, 4}};
    la::dmatrix<int> b = {{1, 2}, {3, 4}};
    la::dmatrix<int> c = {{1, 2}, {3, 5}};

    assert(a == b);
    assert(a != c);

    // Different sizes
    la::dmatrix<int> d = {{1, 2, 3}};
    assert(a != d);

    // Cross-engine equality
    la::fmatrix<int, 2, 2> fa = {{1, 2}, {3, 4}};
    assert(a == fa);
}

int main() {
    test_fixed_matrix_construction();
    test_dynamic_matrix_construction();
    test_element_access();
    test_size_queries();
    test_cross_engine_copy();
    test_submatrix_slicing();
    test_matrix_view_operator_bracket();
    test_matrix_view_empty();
    test_matrix_view_size_tuple();
    test_mutable_submatrix();
    test_zeros();
    test_equality_inequality();

    std::cout << "All tests passed (matrix_test)" << std::endl;
    return 0;
}
