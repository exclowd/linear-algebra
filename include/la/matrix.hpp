//
// Created by abakfja on 4/9/21.
//

#ifndef LA_MATRIX_H
#define LA_MATRIX_H

#include <la/matrix_engine.hpp>
#include <la/matrix_view.hpp>
#include <la/matrix_expression.hpp>
#include <la/matrix_operators.hpp>
#include <la/slice.hpp>

namespace la {

template<typename Engine>
class matrix : public base_matrix_expression<matrix<Engine>> {
public:
    using engine_type = Engine;
    using array_type = typename engine_type::array_type;

    using size_type = typename engine_type::size_type;
    using size_tuple = typename engine_type::size_tuple;
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

    using transpose_type = matrix<typename engine_type::transpose_type> &;
    using const_transpose_type = const matrix<typename engine_type::transpose_type> &;

    using slice_type = slice<size_type>;
    using submatrix_type = matrix<matrix_view_engine<engine_type, detail::read_write_view_tag>>;
    using const_submatrix_type = matrix<matrix_view_engine<engine_type, detail::read_only_view_tag>>;

    /**
     * default destructor
     */
    ~matrix() = default;

    /**
     * Default constructor for fixed-size matrices (value-initializes all elements)
     */
    template<class Engine2 = engine_type, typename = detail::enable_if_static<Engine2>>
    constexpr matrix() : data() {}

    /**
     * default move constructor
     */
    constexpr matrix(matrix &&) noexcept = default;

    /**
     * default copy constructor
     */
    constexpr matrix(const matrix &) = default;

    /**
     * Copy constructor for cross engine conversion of matrices
     * @tparam Engine2 Engine type of the matrix being copied
     * @param other
     */
    template<class Engine2>
    constexpr
    matrix(const matrix<Engine2> &other) : data(other.rows(), other.cols()) {
        data = other;
    }

    /**
     * c-tor for constructing a matrix from a two-dim initializer list
     * @tparam U 
     * @param list 
     */
    template<typename U>
    constexpr matrix(std::initializer_list<std::initializer_list<U>> list): data(list) {}

    /**
     * c-tor for constructing a dynamic_size_matrix of size [row][column]
     * and fill with value v;
     * @tparam Engine2 
     * @param row rows of the matrix
     * @param col columns
     * @param v initial value of the matrix elements
     */
    template<typename Engine2=engine_type, typename = detail::enable_if_dynamic<Engine2>>
    explicit constexpr matrix(size_type r, size_type c): data(r, c) {
    }

    /**
     * c-tor for constructing a dynamic_size_matrix of row * column
     * but using std::pair and fill with value v;
     * @tparam Engine2
     * @param sz A std::pair with the rows as the first element
     * @param v initial value of the matrix elements
     */
    template<typename Engine2=engine_type, typename = detail::enable_if_dynamic<Engine2>>
    explicit constexpr matrix(size_tuple sz): data(sz.first, sz.second) {
    }

    template<typename E>
    constexpr
    matrix(const base_matrix_expression<E> &expr): data(expr.size().first, expr.size().second) {
        detail::check_engine_size(expr, rows(), cols());
        for (size_type i{}; i < rows(); i++) {
            for (size_type j{}; j < cols(); j++) {
                this->operator()(i, j) = expr(i, j);
            }
        }
    }

    constexpr matrix &operator=(matrix &&) noexcept = default;

    constexpr matrix &operator=(matrix const &) = default;

    /**
     * Assignment operator to convert from a matrix of a different engine
     * @tparam engine2
     * @param other
     */
    template<typename engine2>
    constexpr matrix &operator=(const matrix<engine2> &other) {
        if constexpr (detail::is_dynamic_v<engine_type>) {
            if (other.rows() != rows() || other.cols() != cols()) {
                data.resize(other.rows(), other.cols());
            }
        } else {
            detail::check_engine_size(other.data, rows(), cols());
        }
        data = other;
        return *this;
    }

    /**
     * Assignment operator to convert a matrix expression into a matrix type
     *
     * @tparam operation
     * @tparam operands
     * @param expr
     * @return
     */
    template<typename E>
    constexpr matrix &operator=(const base_matrix_expression<E> &expr) {
        if constexpr (detail::is_dynamic_v<engine_type>) {
            if (expr.rows() != rows() || expr.cols() != cols()) {
                data.resize(expr.rows(), expr.cols());
            }
        } else {
            detail::check_engine_size(expr, rows(), cols());
        }
        for (size_type i{}; i < rows(); i++) {
            for (size_type j{}; j < cols(); j++) {
                this->operator()(i, j) = expr(i, j);
            }
        }
        return *this;
    }

    template<typename engine2>
    constexpr bool operator==(const matrix<engine2> &other) const {
        if (rows() != other.rows() || cols() != other.cols()) {
            return false;
        }
        for (size_type i{}; i < rows(); i++) {
            for (size_type j{}; j < cols(); j++) {
                if (this->operator()(i, j) != other(i, j)) {
                    return false;
                }
            }
        }
        return true;
    }


    template<typename E>
    constexpr bool operator==(const base_matrix_expression<E> &other) const {
        if (rows() != other.rows() || cols() != other.cols()) {
            return false;
        }
        for (size_type i{}; i < rows(); i++) {
            for (size_type j{}; j < cols(); j++) {
                if (this->operator()(i, j) != other(i, j)) {
                    return false;
                }
            }
        }
        return true;
    }

    template<typename engine2>
    constexpr bool operator!=(const matrix<engine2> &other) const {
        return !(this->template operator==(other));
    }

//    constexpr column_vector_type operator()(size_type c) const {
//        return column_vector_type(detail::contructor_tag{}, data, c);
//    }
//
//    constexpr row_vector_type operator()(size_type r) const {
//        return row_vector_type(detail::contructor_tag{}, data, r);
//    }

    constexpr const_reference operator[](size_type idx) const noexcept {
        return data[idx];
    }

    constexpr reference operator[](size_type idx) noexcept {
        return data[idx];
    }

    constexpr const_reference operator()(size_type r, size_type c) const noexcept {
        return data(r, c);
    }


    constexpr reference operator()(size_type r, size_type c) noexcept {
        return data(r, c);
    }

    constexpr const_reference operator()(size_tuple sz) const noexcept {
        auto[r, c] = sz;
        return data(r, c);
    }


    constexpr reference operator()(size_tuple sz) noexcept {
        auto[r, c] = sz;
        return data(r, c);
    }

    constexpr const_submatrix_type operator()(size_type r, slice_type c) const noexcept {
        return const_submatrix_type(detail::constructor_tag{}, data, r, 1, c.start(), c.size());
    }

    constexpr const_submatrix_type operator()(slice_type r, size_type c) const noexcept {
        return const_submatrix_type(detail::constructor_tag{}, data, r.start(), r.size(), c, 1);
    }

    constexpr const_submatrix_type operator()(slice_type r, slice_type c) const noexcept {
        return const_submatrix_type(detail::constructor_tag{}, data, r.start(), r.size(), c.start(),
                                    c.size());
    }

    constexpr submatrix_type operator()(size_type r, slice_type c) noexcept {
        return submatrix_type(detail::constructor_tag{}, data, r, 1, c.start(), c.size());
    }

    constexpr submatrix_type operator()(slice_type r, size_type c) noexcept {
        return submatrix_type(detail::constructor_tag{}, data, r.start(), r.size(), c, 1);
    }

    constexpr submatrix_type operator()(slice_type r, slice_type c) noexcept {
        return submatrix_type(detail::constructor_tag{}, data, r.start(), r.size(), c.start(),
                              c.size());
    }

    [[nodiscard]] constexpr size_type data_size() const noexcept {
        return data.data_size();
    }

    [[nodiscard]] constexpr size_tuple size() const noexcept {
        return {data.rows(), data.cols()};
    }

    [[nodiscard]] constexpr size_type rows() const noexcept {
        return data.rows();
    }

    [[nodiscard]] constexpr size_type cols() const noexcept {
        return data.cols();
    }

    [[nodiscard]] constexpr bool empty() const noexcept {
        return data.empty();
    }

    template<class Engine2 = engine_type, typename = detail::enable_if_dynamic<Engine2>>
    void resize(size_type r, size_type c) {
        data.resize(r, c);
    }

    template<class Engine2 = engine_type, typename = detail::enable_if_dynamic<Engine2>>
    void resize(size_tuple sz) {
        data.resize(sz.first, sz.second);
    }

private:
    template<class Engine2, class Layout2>
    friend
    class vector;

    template<class Engine2>
    friend
    class matrix;


    template<class Engine2, class... args>
    constexpr
    matrix(detail::constructor_tag, Engine2 &&eng, args &&... _args) : data(eng, _args...) {}

    engine_type data;
};

template<typename T>
using dmatrix = matrix<dynamic_matrix_engine<T>>;

template<typename T, std::size_t R, std::size_t C>
using fmatrix = matrix<fixed_matrix_engine<T, R, C>>;


template<typename T>
inline decltype(auto) constexpr
zeros(std::size_t r, std::size_t c) {
    return dmatrix<T>(r, c);
}

template<typename T, std::size_t R, std::size_t C>
inline constexpr auto
zeros() {
    // fixed_matrix_engine default constructor already value-initializes m_data
    return fmatrix<T, R, C>();
}

} // namespace la

#endif // LA_MATRIX_H