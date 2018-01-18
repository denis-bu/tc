#ifndef TC_MATRIX_H
#define TC_MATRIX_H

#include <vector>
#include <stdexcept>

namespace tc {

        template<typename T>
        class Matrix {
        public:
                typedef typename std::vector<T>::size_type size_type;
        private:
                size_type rows_;
                size_type cols_;
                T modulo_;
                std::vector<T> d_;
        public:
                template<typename U>
                static Matrix<U> identity(const U& modulo, typename Matrix<U>::size_type n) {
                        Matrix<U> id(modulo, n, n);
                        for (typename Matrix<U>::size_type i = 0; i < n; ++i) {
                                id(i, i) = static_cast<U>(1); // TODO: type traits
                        }
                        return id;
                }

                Matrix(size_type rows, size_type cols) : rows_(rows), cols_(cols), modulo_(), d_(rows * cols) {	}
                Matrix(const T& modulo, size_type rows, size_type cols) : rows_(rows), cols_(cols), modulo_(modulo), d_(rows * cols) {	}

                size_type rows() const { return rows_; }
                size_type cols() const { return cols_; }
                const T& modulo() const { return modulo_; }

                T& operator()(size_type row, size_type col) {
                        return d_[row * cols_ + col];
                }

                const T& operator()(size_type row, size_type col) const {
                        return d_[row * cols_ + col];
                }

                bool operator==(const Matrix<T>& other) const {
                        if (!(rows_ == other.rows_ && cols_ == other.cols_))
                                return false;
                        for (size_t i = 0; i < rows_; ++i) {
                                for (size_t j = 0; j < cols_; ++j) {
                                        if ((*this)(i, j) != other(i, j))
                                                return false;
                                }
                        }
                        return true;
                }

                void reset(const T& v) {
                        d_.assign(d_.size(), v);
                }

                Matrix<T> identity() const {
                        if (rows_ != cols_)
                                throw std::runtime_error("cols != rows");
                        return identity(modulo_, rows_);
                }

        };

        template<typename T>
        std::ostream& operator<<(std::ostream& os, const Matrix<T>& m) {
                for (size_t i = 0; i < m.rows(); ++i) {
                        for (size_t j = 0; j < m.cols(); ++j) {
                                std::cout << m(i, j) << ", ";
                        }
                        std::cout << std::endl;
                }
                return os;
        }

        template<typename T>
        Matrix<T> operator*(const Matrix<T>& left, const Matrix<T>& right) {
                typedef typename Matrix<T>::size_type st;
                if (left.cols() != right.rows())
                        throw std::runtime_error("left.cols != right.rows");
                if (left.modulo() != right.modulo())
                        throw std::runtime_error("left.modulo != right.modulo");
                bool hasModulo = left.modulo() != T();
                const T& mod = left.modulo();
                Matrix<T> r(mod, left.rows(), right.cols());
                if (hasModulo) {
                        for (st i = 0; i < left.rows(); ++i) {
                                for (st j = 0; j < right.cols(); ++j) {
                                        for (st k = 0; k < left.cols(); ++k) {
                                                r(i, j) = (r(i, j) + (left(i, k) * right(k, j)) % mod) % mod;
                                        }
                                }
                        }
                }
                else {
                        for (st i = 0; i < left.rows(); ++i) {
                                for (st j = 0; j < right.cols(); ++j) {
                                        for (st k = 0; k < left.cols(); ++k) {
                                                r(i, j) += left(i, k) * right(k, j);
                                        }
                                }
                        }
                }

                return r;
        }

        template<typename T>
        Matrix<T> mpow_recursive(const Matrix<T>& m, unsigned p) {
                if (p == 0) {
                        throw std::runtime_error("Can't raise matrix to power 0!");
                }
                if (p == 1) {
                        return m;
                }
                if (p % 2 == 1) {
                        return mpow(m, p - 1) * m;
                }
                Matrix<T> result = mpow(m, p / 2);
                return result * result;
        }

        template<typename T>
        Matrix<T> mpow(const Matrix<T>& m, unsigned p) {
                if (p == 0)
                        return m.identity();

                auto result = m;
                auto mToPower = m;
                for (unsigned long long power = 2; power <= p; power = power << 1) {
                        mToPower = mToPower * mToPower;
                        if ((power & p) != 0)
                                result = result * mToPower;
                }
                return result;
        }

}

#endif // TC_MATRIX_H
