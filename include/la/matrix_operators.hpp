//
// Created by abakfja on 4/13/21.
//

#ifndef LA_MATRIX_OPERATORS_HPP
#define LA_MATRIX_OPERATORS_HPP

#include <la/matrix_expression.hpp>
#include <la/matrix_engine.hpp>
#include <cmath>

namespace la {
template<typename LHS, typename RHS>
inline decltype(auto) constexpr
operator+(const base_matrix_expression<LHS>&lhs, const base_matrix_expression<RHS>&rhs) {
    if (lhs.rows() != rhs.rows() || lhs.cols() != rhs.cols()) {
        throw std::runtime_error("Invalid matrices for addition");
    }
    auto op = [](auto a, auto b) { return a + b; };
    return matrix_expression<decltype(op), detail::mat_operand_t<LHS>, detail::mat_operand_t<RHS>>(
        op, *static_cast<const LHS *>(&lhs), *static_cast<const RHS *>(&rhs));
}


template<typename LHS, typename RHS>
inline decltype(auto) constexpr
operator-(const base_matrix_expression <LHS> &lhs, const base_matrix_expression <RHS> &rhs) {
    if (lhs.rows() != rhs.rows() || lhs.cols() != rhs.cols()) {
        throw std::runtime_error("Invalid matrices for subtraction");
    }
    auto op = [](auto a, auto b) { return a - b; };
    return matrix_expression<decltype(op), detail::mat_operand_t<LHS>, detail::mat_operand_t<RHS>>(
        op, *static_cast<const LHS *>(&lhs), *static_cast<const RHS *>(&rhs));
}


template<typename LHS, typename RHS, std::enable_if_t<std::is_arithmetic_v<LHS>, bool> = true>
inline decltype(auto) constexpr
operator*(const LHS &lhs, const base_matrix_expression <RHS> &rhs) {
    auto op = [](auto a, auto b) { return a * b; };
    return matrix_expression<decltype(op), const LHS&, detail::mat_operand_t<RHS>>(
        op, lhs, *static_cast<const RHS *>(&rhs));
}


template<typename LHS, typename RHS, std::enable_if_t<std::is_arithmetic_v<RHS>, bool> = true>
inline decltype(auto) constexpr
operator*(const base_matrix_expression <LHS> &lhs, const RHS &rhs) {
    auto op = [](auto a, auto b) { return a * b; };
    return matrix_expression<decltype(op), detail::mat_operand_t<LHS>, const RHS&>(
        op, *static_cast<const LHS *>(&lhs), rhs);
}


template<typename LHS, typename RHS, std::enable_if_t<std::is_arithmetic_v<LHS>, bool> = true>
inline decltype(auto) constexpr
operator/(const LHS &lhs, const base_matrix_expression <RHS> &rhs) {
    auto op = [](auto a, auto b) { return a / b; };
    return matrix_expression<decltype(op), const LHS&, detail::mat_operand_t<RHS>>(
        op, lhs, *static_cast<const RHS *>(&rhs));
}


template<typename LHS, typename RHS, std::enable_if_t<std::is_arithmetic_v<RHS>, bool> = true>
inline decltype(auto) constexpr
operator/(const base_matrix_expression <LHS> &lhs, const RHS &rhs) {
    auto op = [](auto a, auto b) { return a / b; };
    return matrix_expression<decltype(op), detail::mat_operand_t<LHS>, const RHS&>(
        op, *static_cast<const LHS *>(&lhs), rhs);
}

// Fixed-size matrix multiply: returns fmatrix (stack-allocated, no heap)
template<typename T1, typename T2, std::size_t R, std::size_t K, std::size_t C>
inline auto constexpr
operator*(const matrix<fixed_matrix_engine<T1, R, K>> &lhs,
          const matrix<fixed_matrix_engine<T2, K, C>> &rhs) {
    using value_type = decltype(T1{} * T2{});
    matrix<fixed_matrix_engine<value_type, R, C>> temp;
    for (std::size_t i{}; i < R; i++) {
        for (std::size_t k{}; k < K; k++) {
            auto a = lhs(i, k);
            for (std::size_t j{}; j < C; j++) {
                temp(i, j) += a * rhs(k, j);
            }
        }
    }
    return temp;
}

// General matrix multiply: returns dmatrix (heap-allocated)
template<typename LHS, typename RHS>
inline decltype(auto) constexpr
operator*(const base_matrix_expression <LHS> &lhs, const base_matrix_expression <RHS> &rhs) {
    if (lhs.cols() != rhs.rows()) {
        throw std::runtime_error("Matrix multiplication ill-defined");
    }
    using value_type = decltype(lhs(0, 0) * rhs(0, 0));
    auto temp{matrix<dynamic_matrix_engine<value_type>>(lhs.rows(), rhs.cols())};
    for (std::size_t i{}; i < lhs.rows(); i++) {
        for (std::size_t k{}; k < lhs.cols(); k++) {
            auto a = lhs(i, k);
            for (std::size_t j{}; j < rhs.cols(); j++) {
                temp(i, j) += a * rhs(k, j);
            }
        }
    }
    return temp;
}


template<typename T>
inline decltype(auto) constexpr
transpose(const base_matrix_expression <T> &mat) {
    using storage = detail::mat_operand_t<T>;
    return transpose_expression<storage>(*static_cast<const T *>(&mat));
}

/**
 * Calculate the squared Euclidean norm (avoids sqrt)
 */
template<typename ret = float, typename T, std::enable_if_t<std::is_arithmetic_v<ret>, bool> = true>
inline ret constexpr
squared_norm(const base_matrix_expression <T> &mat) noexcept {
    ret res{};
    for (std::size_t i{}; i < mat.rows(); i++) {
        for (std::size_t j{}; j < mat.cols(); j++) {
            res += mat(i, j) * mat(i, j);
        }
    }
    return res;
}

/**
 * Calculate the Euclidean norm of the matrix
 */
template<typename ret = float, typename T, std::enable_if_t<std::is_floating_point_v<ret>, bool> = true>
inline ret constexpr
norm(const base_matrix_expression <T> &mat) {
    return std::sqrt(squared_norm<ret>(mat));
}

}
#endif //LA_MATRIX_OPERATORS_HPP
