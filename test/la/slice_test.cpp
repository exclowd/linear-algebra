#include <la/slice.hpp>
#include <cassert>
#include <iostream>

void test_basic_construction() {
    la::slice<std::size_t> s(2, 5);
    assert(s.start() == 2);
    assert(s.end() == 5);
    assert(s.size() == 3);
}

void test_zero_size() {
    la::slice<std::size_t> s(3, 3);
    assert(s.start() == 3);
    assert(s.end() == 3);
    assert(s.size() == 0);
}

void test_start_greater_than_end() {
    // Edge case: start > end -> size == 0
    la::slice<std::size_t> s(5, 2);
    assert(s.start() == 5);
    assert(s.size() == 0);
}

void test_const_correctness() {
    const la::slice<std::size_t> s(1, 4);
    assert(s.start() == 1);
    assert(s.end() == 4);
    assert(s.size() == 3);
}

void test_equality() {
    la::slice<std::size_t> a(1, 4);
    la::slice<std::size_t> b(1, 4);
    la::slice<std::size_t> c(2, 5);

    assert(a == b);
    assert(a != c);
}

void test_copy() {
    la::slice<std::size_t> a(1, 4);
    la::slice<std::size_t> b(a);
    assert(b.start() == 1);
    assert(b.end() == 4);
    assert(b.size() == 3);

    la::slice<std::size_t> c(std::move(a));
    assert(c.start() == 1);
    assert(c.size() == 3);
}

void test_full_range() {
    la::slice<std::size_t> s(0, 100);
    assert(s.start() == 0);
    assert(s.end() == 100);
    assert(s.size() == 100);
}

void test_int_type() {
    la::slice<int> s(2, 8);
    assert(s.start() == 2);
    assert(s.end() == 8);
    assert(s.size() == 6);
}

int main() {
    test_basic_construction();
    test_zero_size();
    test_start_greater_than_end();
    test_const_correctness();
    test_equality();
    test_copy();
    test_full_range();
    test_int_type();

    std::cout << "All tests passed (slice_test)" << std::endl;
    return 0;
}
