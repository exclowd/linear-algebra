//
// Created by abakfja on 4/14/21.
//

#ifndef LA_VECTOR_OPERATORS_HPP
#define LA_VECTOR_OPERATORS_HPP

#include <la/vector_expression.hpp>
#include <cmath>
#include <stdexcept>

namespace la {
template<typename LHS, typename RHS>
inline decltype(auto) constexpr
operator+(const base_vector_expression <LHS> &lhs, const base_vector_expression <RHS> &rhs) {
    if (lhs.size() != rhs.size()) {
        throw std::runtime_error("Invalid vectors for addition");
    }
    auto op = [](auto a, auto b) { return a + b; };
    return vector_expression<decltype(op), detail::vec_operand_t<LHS>, detail::vec_operand_t<RHS>>(
        op, *static_cast<const LHS *>(&lhs), *static_cast<const RHS *>(&rhs));
}


template<typename LHS, typename RHS>
inline decltype(auto) constexpr
operator-(const base_vector_expression <LHS> &lhs, const base_vector_expression <RHS> &rhs) {
    if (lhs.size() != rhs.size()) {
        throw std::runtime_error("Invalid vectors for subtraction");
    }
    auto op = [](auto a, auto b) { return a - b; };
    return vector_expression<decltype(op), detail::vec_operand_t<LHS>, detail::vec_operand_t<RHS>>(
        op, *static_cast<const LHS *>(&lhs), *static_cast<const RHS *>(&rhs));
}


template<typename LHS, typename RHS, std::enable_if_t<std::is_arithmetic_v<LHS>, bool> = true>
inline decltype(auto) constexpr
operator*(const LHS &lhs, const base_vector_expression <RHS> &rhs) {
    auto op = [](auto a, auto b) { return a * b; };
    return vector_expression<decltype(op), const LHS&, detail::vec_operand_t<RHS>>(
        op, lhs, *static_cast<const RHS *>(&rhs));
}


template<typename LHS, typename RHS, std::enable_if_t<std::is_arithmetic_v<RHS>, bool> = true>
inline decltype(auto) constexpr
operator*(const base_vector_expression <LHS> &lhs, const RHS &rhs) {
    auto op = [](auto a, auto b) { return a * b; };
    return vector_expression<decltype(op), detail::vec_operand_t<LHS>, const RHS&>(
        op, *static_cast<const LHS *>(&lhs), rhs);
}


template<typename LHS, typename RHS, std::enable_if_t<std::is_arithmetic_v<LHS>, bool> = true>
inline decltype(auto) constexpr
operator/(const LHS &lhs, const base_vector_expression <RHS> &rhs) {
    auto op = [](auto a, auto b) { return a / b; };
    return vector_expression<decltype(op), const LHS&, detail::vec_operand_t<RHS>>(
        op, lhs, *static_cast<const RHS *>(&rhs));
}


template<typename LHS, typename RHS, std::enable_if_t<std::is_arithmetic_v<RHS>, bool> = true>
inline decltype(auto) constexpr
operator/(const base_vector_expression <LHS> &lhs, const RHS &rhs) {
    auto op = [](auto a, auto b) { return a / b; };
    return vector_expression<decltype(op), detail::vec_operand_t<LHS>, const RHS&>(
        op, *static_cast<const LHS *>(&lhs), rhs);
}

template<typename LHS, typename RHS>
inline decltype(auto) constexpr
operator*(const base_vector_expression <LHS> &lhs, const base_vector_expression <RHS> &rhs) {
    if (lhs.size() != rhs.size()) {
        throw std::runtime_error("vector multiplication ill-defined");
    }
    auto op = [](auto a, auto b) { return a * b; };
    return vector_expression<decltype(op), detail::vec_operand_t<LHS>, detail::vec_operand_t<RHS>>(
        op, *static_cast<const LHS *>(&lhs), *static_cast<const RHS *>(&rhs));
}

template<typename LHS, typename RHS>
inline decltype (auto) constexpr
inner_product(const base_vector_expression<LHS> &lhs, const base_vector_expression<RHS> &rhs) {
    if (lhs.size() != rhs.size()) {
        throw std::runtime_error("vector multiplication ill-defined");
    }
    using value_type = decltype(lhs[0] * rhs[0]);
    value_type res{};
    for (std::size_t i{}; i < lhs.size(); i++) {
        res += lhs[i] * rhs[i];
    }
    return res;
}

/**
 * Calculate the squared Euclidean norm (avoids sqrt)
 */
template<typename ret = float, typename T, std::enable_if_t<std::is_arithmetic_v<ret>, bool> = true>
inline ret constexpr
squared_norm(const base_vector_expression <T> &vect) noexcept {
    ret res{};
    for (std::size_t i{}; i < vect.size(); i++) {
        res += vect[i] * vect[i];
    }
    return res;
}

template<typename ret, typename T, std::enable_if_t<std::is_floating_point_v<ret>, bool> = true>
inline ret constexpr
norm(const base_vector_expression <T> &vect) noexcept {
    return std::sqrt(squared_norm<ret>(vect));
}

}
#endif //LA_VECTOR_OPERATORS_HPP
