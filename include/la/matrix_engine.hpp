//
//  Copyright (c) 2021 Kannav Mehta
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef LA_MATRIX_ENGINE_H
#define LA_MATRIX_ENGINE_H

#include <cstddef>
#include <array>
#include <vector>
#include <tuple>
#include <cassert>

#include <la/detail/helper.hpp>
#include <la/traits/storage_traits.hpp>
#include <la/traits/engine_traits.hpp>

namespace la {

template<typename T, std::size_t R, std::size_t C>
struct fixed_matrix_engine {
    static_assert(std::is_object<T>::value,
                  "A matrix's value_type must be an object type (not a "
                  "reference type or void)");

    using array_type = std::array<T, R * C>;
    using storage_traits_type = storage_traits<array_type>;

    using size_type = typename storage_traits_type::size_type;
    using size_tuple = std::pair<size_type, size_type>;
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

    using self_type = fixed_matrix_engine<T, R, C>;
    using transpose_type = fixed_matrix_engine<T, C, R>;

    ~fixed_matrix_engine() noexcept = default;

    fixed_matrix_engine() = default;

    fixed_matrix_engine(const self_type &other) = default;

    fixed_matrix_engine(self_type &&other) noexcept = default;

    template<class Engine2>
    constexpr fixed_matrix_engine(const Engine2 &other): m_data() {
        detail::check_engine_size(other, rows(), cols());
        detail::copy_matrix_engine(*this, other);
    }

    constexpr fixed_matrix_engine(std::initializer_list<std::initializer_list<value_type>> lst) {
        detail::check_init_list(lst, rows(), cols());
        detail::copy_matrix_engine(*this, lst);
    }

    // for compatibility
    constexpr explicit fixed_matrix_engine(size_type r, size_type c) : m_data() {
        if (r != m_rows || c != m_cols) {
            throw std::runtime_error("Cannot reshape a fixed vector, use dynamic vector");
        }
    }

    constexpr fixed_matrix_engine &operator=(fixed_matrix_engine &&) noexcept = default;

    constexpr fixed_matrix_engine &operator=(fixed_matrix_engine const &) = default;

    constexpr reference operator[](size_type idx) noexcept {
        return m_data[idx];
    }

    constexpr const_reference operator[](size_type idx) const noexcept {
        return m_data[idx];
    }

    constexpr reference operator()(size_type r, size_type c) noexcept {
        return m_data[c + r * cols()];
    }

    constexpr const_reference operator()(size_type r, size_type c) const noexcept {
        return m_data[c + r * cols()];
    }

    [[nodiscard]] constexpr size_type rows() const noexcept {
        return m_rows;
    }

    [[nodiscard]] constexpr size_type cols() const noexcept {
        return m_cols;
    }

    [[nodiscard]] constexpr size_type data_size() const noexcept {
        return m_data.size();
    }

    [[nodiscard]] constexpr bool empty() const noexcept {
        return m_data.empty();
    }

    constexpr pointer data() noexcept { return m_data.data(); }
    constexpr const_pointer data() const noexcept { return m_data.data(); }

    constexpr iterator begin() noexcept { return m_data.begin(); }
    constexpr iterator end() noexcept { return m_data.end(); }
    constexpr const_iterator begin() const noexcept { return m_data.begin(); }
    constexpr const_iterator end() const noexcept { return m_data.end(); }

    static constexpr size_type m_rows{R};
    static constexpr size_type m_cols{C};

    array_type m_data;
};

template<typename T>
struct dynamic_matrix_engine {
    static_assert(std::is_object<T>::value,
                  "A vector's value_type must be an object type (not a "
                  "reference type or void)");
    using scalar_type = T;

    using array_type = std::vector<scalar_type>;
    using storage_traits_type = storage_traits<array_type>;

    using size_type = typename storage_traits_type::size_type;
    using size_tuple = std::pair<size_type, size_type>;
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

    using self_type = dynamic_matrix_engine<T>;
    using transpose_type = self_type;

    ~dynamic_matrix_engine() noexcept = default;

    dynamic_matrix_engine() = default;

    dynamic_matrix_engine(const self_type &other) = default;

    dynamic_matrix_engine(self_type &&other) noexcept = default;

    template<class Engine2>
    constexpr
    dynamic_matrix_engine(const Engine2 &other) : m_rows{other.rows()}, m_cols{other.cols()},
                                                  m_data(m_rows * m_cols) {
        detail::copy_matrix_engine(*this, other);
    }

    template<typename U>
    dynamic_matrix_engine(const std::initializer_list<std::initializer_list<U>> &lst) :
            m_rows{lst.size()},
            m_cols{lst.begin()->size()},
            m_data(m_rows * m_cols) {
        detail::check_init_list(lst);
        detail::copy_matrix_engine(*this, lst);
    }

    explicit dynamic_matrix_engine(size_type r, size_type c) : m_rows{r}, m_cols{c},
                                                               m_data(r * c) {}


    constexpr dynamic_matrix_engine &operator=(dynamic_matrix_engine &&) noexcept = default;

    constexpr dynamic_matrix_engine &operator=(dynamic_matrix_engine const &) = default;

    template<typename Engine2>
    dynamic_matrix_engine &operator=(const Engine2 &other) {
        detail::check_engine_size(other, rows(), cols());
        detail::copy_matrix_engine(*this, other);
        return *this;
    }


    size_type rows() const noexcept {
        return m_rows;
    }

    size_type cols() const noexcept {
        return m_cols;
    }

    reference operator[](size_type idx) noexcept {
        return m_data[idx];
    }

    const_reference operator[](size_type idx) const noexcept {
        return m_data[idx];
    }

    reference operator()(size_type r, size_type c) noexcept {
        return m_data[c + r * cols()];
    }

    const_reference operator()(size_type r, size_type c) const noexcept {
        return m_data[c + r * cols()];
    }

    [[nodiscard]] size_tuple size() const noexcept {
        return std::make_pair(m_rows, m_cols);
    }

    [[nodiscard]] size_type data_size() const noexcept {
        return m_data.size();
    }

    void resize(size_type r, size_type c) {
        m_data.resize(r * c);
        m_rows = r;
        m_cols = c;
    }

    [[nodiscard]] bool empty() const noexcept {
        return m_data.empty();
    }

    pointer data() noexcept { return m_data.data(); }
    const_pointer data() const noexcept { return m_data.data(); }

    iterator begin() noexcept { return m_data.begin(); }
    iterator end() noexcept { return m_data.end(); }
    const_iterator begin() const noexcept { return m_data.begin(); }
    const_iterator end() const noexcept { return m_data.end(); }

    size_type m_rows;
    size_type m_cols;
    array_type m_data;
};


namespace detail {
template<typename T, std::size_t R, std::size_t C>
struct is_static<fixed_matrix_engine<T, R, C>> : std::true_type {
};

template<typename T>
struct is_dynamic<dynamic_matrix_engine<T>> : std::true_type {
};
} // detail

} // namespace la
#endif // LA_MATRIX_ENGINE_H
