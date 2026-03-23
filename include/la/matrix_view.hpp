//
// Created by abakfja on 4/9/21.
//

#ifndef LA_MATRIX_VIEW_HPP
#define LA_MATRIX_VIEW_HPP

#include <la/detail/helper.hpp>
#include <la/traits/read_write_traits.hpp>

namespace la {

/**
 * A non-owning view for a matrix(i.e 1D contiguous range)
 * @tparam Engine The matrix_view_engine uses the engine type as a traits type and not as a data
 *         source
 * @tparam read_write_tag
 * @tparam view_tag
 */
template<typename Engine, class read_write_tag>
struct matrix_view_engine {
    using engine_type = Engine;
    using array_type = typename engine_type::array_type;

    using size_type = typename engine_type::size_type;
    using size_tuple = typename engine_type::size_tuple;
    using difference_type = typename engine_type::difference_type;

    using value_type = typename engine_type::value_type;

    using read_write_traits_type = read_write_traits<engine_type, read_write_tag>;
    using reference = typename read_write_traits_type::reference;
    using engine_reference = typename read_write_traits_type::engine_reference;
    using const_reference = typename engine_type::const_reference;

    using pointer = typename read_write_traits<engine_type, read_write_tag>::pointer;
    using const_pointer = typename engine_type::const_pointer;

    using iterator = typename engine_type::iterator;
    using const_iterator = typename engine_type::const_iterator;

    using reverse_iterator = typename engine_type::reverse_iterator;
    using const_reverse_iterator = typename engine_type::const_reverse_iterator;

    using storage_tag = typename engine_type::storage_tag;

    using transpose_type = matrix_view_engine<typename engine_type::transpose_type,
            read_write_tag> &;
    using const_transpose_type = const matrix_view_engine<typename engine_type::transpose_type,
            read_write_tag> &;

    ~matrix_view_engine() noexcept = default;

    constexpr matrix_view_engine() noexcept = default;

    constexpr matrix_view_engine(matrix_view_engine &&) noexcept = default;

    constexpr matrix_view_engine(const matrix_view_engine &) noexcept = default;

    constexpr matrix_view_engine(engine_reference &engine, size_type r_start, size_type r_count,
                                 size_type c_start, size_type c_count) :
            data(&engine),
            m_row_start{r_start},
            m_row_elems{r_count},
            m_col_start{c_start},
            m_col_elems{c_count} {
    }

    constexpr matrix_view_engine &operator=(matrix_view_engine &&other) {
        if (&other != this) {
            detail::check_engine_size(other, rows(), cols());
            detail::copy_matrix_engine(*this, other);
        }
        return *this;
    }

    constexpr matrix_view_engine &operator=(const matrix_view_engine &other) {
        if (&other != this) {
            detail::check_engine_size(other, rows(), cols());
            detail::copy_matrix_engine(*this, other);
        }
        return *this;
    }

    template<typename Engine2>
    constexpr matrix_view_engine &operator=(const Engine2 &other) {
        detail::check_engine_size(other, rows(), cols());
        detail::copy_matrix_engine(*this, other);
        return *this;
    }

    constexpr const_reference operator[](size_type idx) const noexcept {
        return this->operator()(idx / cols(), idx % cols());
    }

    constexpr reference operator[](size_type idx) noexcept {
        return this->operator()(idx / cols(), idx % cols());
    }


    constexpr const_reference operator()(size_type r, size_type c) const noexcept {
        return data->operator()(m_row_start + r, m_col_start + c);
    }


    constexpr reference operator()(size_type r, size_type c) noexcept {
        return data->operator()(m_row_start + r, m_col_start + c);
    }

    constexpr size_type rows() const noexcept {
        return m_row_elems;
    }

    constexpr size_type cols() const noexcept {
        return m_col_elems;
    }

    [[nodiscard]] constexpr size_tuple size() const noexcept {
        return size_tuple(m_row_elems, m_col_elems);
    }

    [[nodiscard]] constexpr bool empty() const noexcept {
        return m_row_elems * m_col_elems == 0;
    }

    engine_reference *data;
    size_type m_row_start;
    size_type m_row_elems;
    size_type m_col_start;
    size_type m_col_elems;
};

}
#endif //LA_MATRIX_VIEW_HPP
