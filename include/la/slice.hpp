//
// Created by abakfja on 4/13/21.
//

#ifndef LA_SLICE_HPP
#define LA_SLICE_HPP

#include <cstddef>
#include <tuple>

namespace la {

template<typename T = std::size_t>
struct slice {

    slice() = delete;

    constexpr slice(T start, T end) : m_start{start}, m_elems{end - start} {
        if (start > end) {
            m_elems = 0;
        }
    }

    constexpr slice(const slice &) noexcept = default;

    constexpr slice(slice &&) noexcept = default;

    constexpr bool operator==(const slice &rhs) const noexcept {
        return m_start == rhs.m_start && m_elems == rhs.m_elems;
    }

    constexpr bool operator!=(const slice &rhs) const noexcept {
        return !(*this == rhs);
    }

    constexpr T size() const noexcept {
        return m_elems;
    }

    constexpr T end() const noexcept {
        return m_start + m_elems;
    }

    constexpr T start() const noexcept {
        return m_start;
    }

    T m_start;
    T m_elems;
};
}

#endif //LA_SLICE_HPP
