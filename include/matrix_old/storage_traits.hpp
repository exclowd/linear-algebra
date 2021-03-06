//
// Created by abakfja on 3/28/21.
//

#ifndef LA_STORAGE_TRAITS_HPP
#define LA_STORAGE_TRAITS_HPP

#include <vector>
#include <array>

namespace boost::numeric::ublas::experimental {
struct storage_resizable_container_tag{};
struct storage_static_container_tag{};
}

namespace boost::numeric::ublas::experimental {

template<class A>
struct storage_traits;


template<typename T>
struct storage_traits<std::vector<T>> {
    using storage_type = std::vector<T>;

    using size_type = typename storage_type::size_type;
    using difference_type = typename storage_type::difference_type;
    using value_type = typename storage_type::value_type;

    using reference = typename storage_type::reference;
    using const_reference = typename storage_type::const_reference;

    using pointer = typename storage_type::pointer;
    using const_pointer = typename storage_type::const_pointer;

    using iterator = typename storage_type::iterator;
    using const_iterator = typename storage_type::const_iterator;

    using reverse_iterator = typename storage_type::reverse_iterator;
    using const_reverse_iterator = typename storage_type::const_reverse_iterator;

    using resizable_tag = storage_resizable_container_tag;

};


template<typename T, std::size_t N>
struct storage_traits<std::array<T, N>> {
    using storage_type = std::array<T, N>;

    using size_type = typename storage_type::size_type;
    using difference_type = typename storage_type::difference_type;
    using value_type = typename storage_type::value_type;

    using reference = typename storage_type::reference;
    using const_reference = typename storage_type::const_reference;

    using pointer = typename storage_type::pointer;
    using const_pointer = typename storage_type::const_pointer;

    using iterator = typename storage_type::iterator;
    using const_iterator = typename storage_type::const_iterator;

    using reverse_iterator = typename storage_type::reverse_iterator;
    using const_reverse_iterator = typename storage_type::const_reverse_iterator;

    using resizable_tag = storage_static_container_tag;
};


}
#endif //LA_STORAGE_TRAITS_HPP
