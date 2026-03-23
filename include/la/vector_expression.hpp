//
// Created by abakfja on 4/4/21.
//
//
#ifndef LA_VECTOR_EXPRESSION_HPP
#define LA_VECTOR_EXPRESSION_HPP

#include <algorithm>
#include <la/traits/expression_traits.hpp>
#include <tuple>


namespace la {

template<typename E>
struct base_vector_expression {
    using expression_type = E;

    constexpr auto operator[](std::size_t x) const {
        return static_cast<const E &>(*this)[x];
    }

    constexpr auto size() const {
        return static_cast<const E &>(*this).size();
    }
};

namespace detail {

template<typename operand>
constexpr inline auto vec_subscript(const operand &a, std::size_t x) {
    if constexpr(detail::is_vector_expr_v<std::decay_t<operand>>) {
        return a[x];
    } else {
        return a;
    }
}

template<typename operand>
constexpr inline std::size_t vec_expr_size(const operand &a) {
    if constexpr(detail::is_vector_expr_v<std::decay_t<operand>>) {
        return a.size();
    } else {
        return 0;
    }
}

} // namespace detail

// operands encode their own storage: const T& for concrete types, T for lazy expressions.
// The operators are responsible for selecting the right storage via detail::vec_operand_t.
template<class operation, typename ... operands>
struct vector_expression
        : public base_vector_expression<vector_expression<operation, operands...>> {

    ~vector_expression() = default;

    vector_expression() = default;

    constexpr vector_expression(const vector_expression &other) = default;

    constexpr vector_expression(vector_expression &&other) noexcept = default;

    constexpr explicit vector_expression(operation f_, const operands &... args_) : f(f_), args(args_...) {}

    constexpr vector_expression &operator=(vector_expression &&l) noexcept = default;

    constexpr vector_expression &operator=(vector_expression const &l) = default;

    constexpr auto operator[](std::size_t idx) const {
        return std::apply([this, idx](const auto &... a) {
            return f(detail::vec_subscript(a, idx)...);
        }, args);
    }

    constexpr auto size() const {
        return std::apply([this](const auto &... x) {
            std::size_t a{};
            ((a = std::max(a, static_cast<std::size_t>(detail::vec_expr_size(x)))), ...);
            return a;
        }, args);
    }


    operation f;
    std::tuple<operands...> args;
};

} // namespace la

#endif //LA_VECTOR_EXPRESSION_HPP
