//
//  Copyright (c) 2021 Kannav Mehta
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef LA_VECTOR_H
#define LA_VECTOR_H

#include <la/detail/helper.hpp>
#include <la/vector_engine.hpp>
#include <la/vector_expression.hpp>
#include <la/vector_operators.hpp>
#include <la/vector_view.hpp>
#include <la/traits/layout_traits.hpp>
#include <la/slice.hpp>


namespace la {

template<typename Engine, typename Layout = layout::row_major>
class vector : public base_vector_expression<vector<Engine, Layout>> {
public:
    using engine_type = Engine;
    using layout_type = Layout;
    using array_type = typename engine_type::array_type;

    using size_type = typename engine_type::size_type;
    using difference_type = typename engine_type::difference_type;

    using value_type = typename engine_type::value_type;

    using reference = typename engine_type::reference;
    using const_reference = typename engine_type::const_reference;

    using pointer = typename engine_type::pointer;
    using const_pointer = typename engine_type::const_pointer;

    using iterator = typename engine_type::iterator;
    using const_iterator = typename engine_type::const_iterator;

    using reverse_iterator = typename engine_type::reverse_iterator;
    using const_reverse_iterator = typename engine_type::const_reverse_iterator;

    using storage_tag = typename engine_type::storage_tag;

    using transpose_type = vector<typename engine_type::transpose_type,
            typename layout_type::transpose_type> &;
    using const_transpose_type = const vector<typename engine_type::transpose_type,
            typename layout_type::transpose_type> &;

    using slice_type = slice<size_type>;
    using subvector_type = vector<vector_view_engine < engine_type, detail::read_write_view_tag>>;
    using const_subvector_type = vector<
            vector_view_engine < engine_type, detail::read_only_view_tag>>;

    /**
     * default destructor
     */
    ~vector() = default;

    /**
     * Default constructor for fixed-size vectors (value-initializes all elements)
     */
    template<class Engine2 = engine_type, typename = detail::enable_if_static<Engine2>>
    constexpr vector() : data() {}

    /**
     * default move constructor
     */
    constexpr vector(vector &&) noexcept = default;

    /**
     * default copy constructor
     */
    constexpr vector(const vector &) = default;

    /**
     * Copy constructor for cross engine conversion of vectors
     * @tparam Engine2 Engine type of the matrix being copied
     * @param other
     */
    template<class Engine2>
    constexpr
    vector(const vector<Engine2, layout_type> &other) : data(other.size()) {
        data = other;
    }

    /**
     * c-tor for constructing a vector initializer list. Bounds are checked.
     * @tparam U
     * @param list
     */
    template<typename U>
    constexpr vector(std::initializer_list <U> list): data(list) {}

    /**
     * c-tor for constructing a dynamic_size_vector of size n
     * and fill with value v;
     * @tparam Engine2
     * @param n number of elements
     * @param v initial value of the vector elements
     */
    template<class Engine2 = engine_type, typename = detail::enable_if_dynamic <Engine2>>
    explicit constexpr vector(size_type n) : data(n) {
    }

    template<typename E>
    constexpr
    vector(const base_vector_expression <E> &expr): data(expr.size()) {
        detail::check_engine_size(expr, size());
        for (size_type i = 0; i < size(); i++) {
            this->operator[](i) = expr[i];
        }
    }

    constexpr vector &operator=(vector &&) noexcept = default;

    constexpr vector &operator=(vector const &) = default;

    // copy assignment
    template<typename Engine2>
    constexpr vector &operator=(const vector<Engine2, layout_type> &other) {
        if constexpr (detail::is_dynamic_v < engine_type >) {
            if (other.size() != size()) {
                data.resize(other.size());
            }
        } else {
            detail::check_engine_size(other.data, size());
        }
        data = other;
        return *this;
    }

    template<typename E>
    constexpr vector &operator=(const base_vector_expression <E> &expr) {
        if constexpr (detail::is_dynamic_v < engine_type >) {
            if (expr.size() != size()) {
                data.resize(expr.size());
            }
        } else {
            detail::check_engine_size(expr, size());
        }
        for (size_type i = 0; i < size(); i++) {
            this->operator[](i) = static_cast<value_type>(expr[i]);
        }
        return *this;
    }

    template<typename engine2>
    bool operator==(const vector<engine2, layout_type> &other) const {
        if (size() != other.size()) {
            return false;
        }
        for (size_type i = 0; i < size(); i++) {
            if (this->operator[](i) != other[i]) {
                return false;
            }
        }
        return true;
    }


    template<typename E>
    constexpr bool operator==(const base_vector_expression <E> &other) const {
        if (size() != other.size()) {
            return false;
        }
        for (size_type i = 0; i < size(); i++) {
            if (this->operator[](i) != other[i]) {
                return false;
            }
        }
        return true;
    }

    template<typename engine2>
    constexpr bool operator!=(const vector<engine2, layout_type> &other) const {
        return !(this->template operator==(other));
    }

    constexpr const_subvector_type
    operator()(slice_type slice) const {
        return const_subvector_type(detail::constructor_tag{}, data,
                                    slice.start(),
                                    slice.size());
    }


    constexpr subvector_type
    operator()(slice_type slice) {
        return subvector_type(detail::constructor_tag{}, data,
                              slice.start(),
                              slice.size());
    }

    constexpr const_reference operator[](size_type idx) const noexcept {
        return data[idx];
    }

    constexpr reference operator[](size_type idx) noexcept {
        return data[idx];
    }

    [[nodiscard]] constexpr size_type size() const noexcept {
        return data.size();
    }

    [[nodiscard]] constexpr bool empty() const noexcept {
        return data.empty();
    }

    template<class Engine2 = engine_type, typename = detail::enable_if_dynamic <Engine2>>
    void resize(size_type n) {
        data.resize(n);
    }

    constexpr auto begin() noexcept {
        return data.begin();
    }

    constexpr auto end() noexcept {
        return data.end();
    }

    constexpr auto begin() const noexcept {
        return data.begin();
    }

    constexpr auto end() const noexcept {
        return data.end();
    }

private:
    template<class Engine2, class layout2>
    friend
    class vector;

    template<class Engine2>
    friend
    class matrix;

    template<class Engine2, class... args>
    constexpr
    vector(detail::constructor_tag, Engine2 &&eng, args &&... _args) : data(eng, _args...) {
    }

    engine_type data;
};

template<typename T>
using dvector = vector<dynamic_vector_engine < T>>;

template<typename T, std::size_t N>
using fvector = vector<fixed_vector_engine < T, N>>;

template<typename T>
using d_col_vector = vector<dynamic_vector_engine < T>, layout::column_major>;

template<typename T, std::size_t N>
using f_col_vector = vector<fixed_vector_engine < T, N>, layout::column_major>;

template<typename T>
inline decltype(auto) constexpr
zeros(std::size_t n) {
    return dvector<T>(n);
}

template<typename T, std::size_t N>
inline constexpr auto
zeros() {
    // fixed_vector_engine default constructor already value-initializes m_data
    return fvector<T, N>();
}

} // namespace la

#endif // LA_VECTOR_H