//
// Created by abakfja on 3/28/21.
//

#ifndef LA_ENGINE_TRAITS_HPP
#define LA_ENGINE_TRAITS_HPP

#include <cstddef>
#include <type_traits>
namespace la {

template<typename T>
struct dynamic_matrix_engine;

template<typename T, std::size_t R, std::size_t C>
struct fixed_matrix_engine;

template<typename T>
struct dynamic_vector_engine;

template<typename T, std::size_t N>
struct fixed_vector_engine;

}

namespace la::detail {

/// @brief checks if type is a matrix
template<typename T>
struct is_static : std::false_type {
};

template<typename T>
using is_static_t = typename is_static<std::decay_t<T>>::type;

template<typename T>
inline constexpr bool is_static_v = is_static_t<T>::value;

template<typename T>
struct is_static<T &> : is_static<T> {
};

/// @brief used for type space reduction in expression templates
template<typename T>
using enable_if_static = typename std::enable_if_t<is_static_v<T>, bool>;



/// @brief checks if type is a matrix
template<typename T>
struct is_dynamic : std::false_type {
};

template<typename T>
using is_dynamic_t = typename is_dynamic<std::decay_t<T>>::type;

template<typename T>
inline constexpr bool is_dynamic_v = is_dynamic_t<T>::value;

template<typename T>
struct is_dynamic<T &> : is_dynamic<T> {
};

/// @brief used for type space reduction in expression templates
template<typename T>
using enable_if_dynamic = typename std::enable_if_t<is_dynamic_v<T>, bool>;

} // namespace la::detail


#endif //LA_ENGINE_TRAITS_HPP
