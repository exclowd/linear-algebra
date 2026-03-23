//
// Created by fragrant on 4/9/21.
//

#ifndef LA_VECTOR_VIEW_HPP
#define LA_VECTOR_VIEW_HPP

#include <la/detail/helper.hpp>
#include <la/traits/read_write_traits.hpp>
#include <algorithm>

namespace la {

/**
 * A non-owning view for a vector(i.e 1D contiguous range)
 * @tparam Engine The vector_view_engine uses the engine type as a traits type and not as a data
 *         source
 * @tparam read_write_tag
 * @tparam view_tag
 */
template<typename Engine, class read_write_tag>
struct vector_view_engine {
    using engine_type = Engine;
    using array_type = typename engine_type::array_type;

    using size_type = typename engine_type::size_type;
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

    using transpose_type = vector_view_engine<typename engine_type::transpose_type, read_write_tag> &;
    using const_transpose_type = const vector_view_engine<typename engine_type::transpose_type, read_write_tag> &;

    ~vector_view_engine() noexcept = default;

    constexpr vector_view_engine() noexcept = default;

    constexpr vector_view_engine(vector_view_engine &&) noexcept = default;

    constexpr vector_view_engine(const vector_view_engine &) noexcept = default;

    constexpr vector_view_engine(engine_reference &engine, size_type start, size_type count) :
            data(&engine), m_start{start}, m_elems{count} {
    }

    constexpr vector_view_engine &operator=(vector_view_engine &&other) {
        if (&other != this) {
            detail::check_engine_size(other, size());
            std::copy(other.begin(), other.end(), begin());
        }
        return *this;
    }

    constexpr vector_view_engine &operator=(const vector_view_engine &other) {
        if (&other != this) {
            detail::check_engine_size(other, size());
            std::copy(other.begin(), other.end(), begin());
        }
        return *this;
    }

    template<typename Engine2>
    constexpr vector_view_engine &operator=(const Engine2& other) {
        detail::check_engine_size(other, size());
        detail::copy_vector_engine(*this, other);
        return *this;
    }

    constexpr const_reference operator[](size_type idx) const noexcept {
        return data->operator[](m_start + idx);
    }

    constexpr reference operator[](size_type idx) noexcept {
        return data->operator[](m_start + idx);
    }

    [[nodiscard]] constexpr size_type size() const noexcept {
        return m_elems;
    }

    [[nodiscard]] constexpr bool empty() const noexcept {
        return m_elems == 0;
    }

    constexpr auto begin() noexcept {
        return data->begin() + m_start;
    }

    constexpr auto end() noexcept {
        return data->begin() + m_start + m_elems;
    }

    constexpr auto begin() const noexcept {
        return data->begin() + m_start;
    }

    constexpr auto end() const noexcept {
        return data->begin() + m_start + m_elems;
    }

    engine_reference *data = nullptr;
    size_type m_start{};
    size_type m_elems{};
};

}
#endif //LA_VECTOR_VIEW_HPP
