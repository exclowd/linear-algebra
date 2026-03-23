//
//  Copyright (c) 2021 Kannav Mehta
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef LA_VECTOR_ENGINE_H
#define LA_VECTOR_ENGINE_H

#include <cstddef>
#include <array>
#include <vector>
#include <tuple>
#include <cassert>

#include <la/detail/helper.hpp>
#include <la/traits/storage_traits.hpp>
#include <la/traits/engine_traits.hpp>

namespace la {

template<typename T, std::size_t N>
struct fixed_vector_engine {
    static_assert(std::is_object<T>::value,
                  "A vector's value_type must be an object type (not a "
                  "reference type or void)");

    using array_type = std::array<T, N>;
    using storage_traits_type = storage_traits<array_type>;

    using size_type = typename storage_traits_type::size_type;
    using difference_type = typename storage_traits_type::difference_type;

    using value_type = typename storage_traits_type::value_type;

    using reference = typename storage_traits_type::reference;
    using const_reference = typename storage_traits_type::const_reference;

    using pointer = typename storage_traits_type::pointer;
    using const_pointer = typename storage_traits_type::const_pointer;

    using iterator = typename storage_traits_type::iterator;
    using const_iterator = typename storage_traits_type::const_iterator;

    using reverse_iterator = typename storage_traits_type::reverse_iterator;
    using const_reverse_iterator = typename storage_traits_type::const_reverse_iterator;

    using storage_tag = typename storage_traits_type::storage_tag;

    using self_type = fixed_vector_engine<T, N>;
    using transpose_type = fixed_vector_engine<T, N>;

    ~fixed_vector_engine() noexcept = default;

    constexpr fixed_vector_engine() = default;

    constexpr fixed_vector_engine(const self_type &other) = default;

    constexpr fixed_vector_engine(self_type &&other) noexcept = default;

    template<class Engine2>
    constexpr fixed_vector_engine(const Engine2 &other): m_data() {
        detail::check_engine_size(other, size());
        detail::copy_vector_engine(*this, other);
    }

    template<typename U>
    constexpr fixed_vector_engine(std::initializer_list<U> lst) {
        detail::check_init_list(lst, size());
        detail::copy_vector_engine(*this, lst);
    }

    constexpr explicit fixed_vector_engine(size_type s) : m_data() {
        if (s != m_elems) {
            throw std::runtime_error("Cannot reshape a fixed vector, use dynamic vector");
        }
        std::fill(m_data.begin(), m_data.end(), value_type{});
    }

    constexpr fixed_vector_engine &operator=(fixed_vector_engine &&) noexcept = default;

    constexpr fixed_vector_engine &operator=(fixed_vector_engine const &) = default;

    constexpr reference operator[](size_type idx) noexcept {
        return m_data[idx];
    }

    constexpr const_reference operator[](size_type idx) const noexcept {
        return m_data[idx];
    }

    [[nodiscard]] constexpr size_type size() const noexcept {
        return m_data.size();
    }

    [[nodiscard]] constexpr bool empty() const noexcept {
        return m_data.empty();
    }

    constexpr pointer data() noexcept { return m_data.data(); }
    constexpr const_pointer data() const noexcept { return m_data.data(); }

    constexpr auto begin() noexcept { return m_data.begin(); }
    constexpr auto end() noexcept { return m_data.end(); }
    constexpr auto begin() const noexcept { return m_data.begin(); }
    constexpr auto end() const noexcept { return m_data.end(); }

    static constexpr size_type m_elems{N};

    array_type m_data;
};

template<typename T>
struct dynamic_vector_engine {
    static_assert(std::is_object<T>::value,
                  "A vector's value_type must be an object type (not a "
                  "reference type or void)");

    using scalar_type = T;

    using array_type = std::vector<scalar_type>;
    using storage_traits_type = storage_traits<array_type>;

    using size_type = typename storage_traits_type::size_type;
    using difference_type = typename storage_traits_type::difference_type;

    using value_type = typename storage_traits_type::value_type;

    using reference = typename storage_traits_type::reference;
    using const_reference = typename storage_traits_type::const_reference;

    using pointer = typename storage_traits_type::pointer;
    using const_pointer = typename storage_traits_type::const_pointer;

    using iterator = typename storage_traits_type::iterator;
    using const_iterator = typename storage_traits_type::const_iterator;

    using reverse_iterator = typename storage_traits_type::reverse_iterator;
    using const_reverse_iterator = typename storage_traits_type::const_reverse_iterator;

    using storage_tag = typename storage_traits_type::storage_tag;

    using self_type = dynamic_vector_engine<T>;
    using transpose_type = self_type;

    ~dynamic_vector_engine() noexcept = default;

    dynamic_vector_engine() = default;

    dynamic_vector_engine(const self_type &other) = default;

    dynamic_vector_engine(self_type &&other) noexcept = default;


    template<class Engine2>
    constexpr dynamic_vector_engine(const Engine2 &other): m_data(other.size()) {
        detail::copy_vector_engine(*this, other);
    }

    template<typename U>
    dynamic_vector_engine(std::initializer_list<U> lst) : m_data(lst.size()) {
        detail::copy_vector_engine(*this, lst);
    }

    explicit dynamic_vector_engine(size_type n) : m_data(n) {
    };

    constexpr dynamic_vector_engine &operator=(dynamic_vector_engine &&) noexcept = default;

    constexpr dynamic_vector_engine &operator=(dynamic_vector_engine const &) = default;

    template<typename Engine2>
    dynamic_vector_engine &operator=(const Engine2 &other) {
        detail::check_engine_size(other, size());
        detail::copy_vector_engine(*this, other);
        return *this;
    }

    reference operator[](size_type idx) noexcept {
        return m_data[idx]; // no bound checking here
    }

    const_reference operator[](size_type idx) const noexcept {
        return m_data[idx]; // no bound checking here
    }

    [[nodiscard]]  size_type size() const noexcept {
        return m_data.size();
    }

    [[nodiscard]]  bool empty() const noexcept {
        return m_data.empty();
    }

    void resize(size_type elems) {
        m_data.resize(elems);
    }

    pointer data() noexcept { return m_data.data(); }
    const_pointer data() const noexcept { return m_data.data(); }

    auto begin() noexcept { return m_data.begin(); }
    auto end() noexcept { return m_data.end(); }
    auto begin() const noexcept { return m_data.begin(); }
    auto end() const noexcept { return m_data.end(); }

    array_type m_data;
};


namespace detail {
template<typename T, std::size_t N>
struct is_static<fixed_vector_engine<T, N>> : std::true_type {
};

template<typename T>
struct is_dynamic<dynamic_vector_engine<T>> : std::true_type {
};
} // detail

} // namespace la

#endif // LA_VECTOR_ENGINE_H