#pragma once
#ifndef MATRIX_H
#define MATRIX_H

#include <cmath>
#include <vector>
#include <stdexcept>
#include <iostream>

using std::vector;
using std::enable_if_t;
using std::is_same;
using std::runtime_error;
using std::ostream;
using std::swap;
using std::pair;
using std::max;

namespace MatrixLib {

    template<typename T>
    constexpr bool is_numeric_v =
        is_same<T, int>::value
        || is_same<T, long double>::value
        || is_same<T, short>::value
        || is_same<T, long long>::value
        || is_same<T, int64_t>::value
        || is_same<T, int32_t>::value
        || is_same<T, int16_t>::value
        || is_same<T, __int64>::value
        || is_same<T, __int32>::value
        || is_same<T, __int16>::value
        || is_same<T, double>::value
        || is_same<T, long double>::value;

    template<typename T, class = enable_if_t<is_numeric_v<T>>>
    class Matrix {
    public:

        Matrix() {
            rows = 0;
            columns = 0;
            matrixLink = &matrix;
            transponentedMatrixLink = &transponentedMatrix;
        }

        Matrix(int rows, int columns) {
            matrixLink = &matrix;
            transponentedMatrixLink = &transponentedMatrix;
            Resize(rows, columns);
        }

        Matrix(int rows, int columns, T defaulv_value) {
            matrixLink = &matrix;
            transponentedMatrixLink = &transponentedMatrix;
            Assigne(rows, columns, defaulv_value);
        }

        void Resize(int rows, int columns) {
            ResizeWithValue(rows, columns, 0);
        }

        void Assigne(int rows, int columns, T defaulv_value) {
            ResizeWithValue(rows, columns, defaulv_value);
        }

        void Transponent(bool inMemory = false) {
            if (inMemory) {
                TransponentInMemory();
                return;
            }
            swap(matrixLink, transponentedMatrixLink);
            swap(rows, columns);
        }

        int Rows() {
            return rows;
        }

        int Columns() {
            return columns;
        }

        vector<T*>& operator[](size_t i) {
            if (i < 0 || i >= rows)
                throw runtime_error("Matrix subscript out of range");
            return (*matrixLink)[i];
        }

        T& operator[](pair<size_t, size_t> i) {
            if (i.first < 0 || i.first >= rows || i.second < 0 || i.second >= columns)
                throw runtime_error("Matrix subscript out of range");
            return *(*matrixLink)[i.first][i.second];
        }

        friend ostream& operator<<(ostream& out, const Matrix<T>& m);

        Matrix<T>& operator+ (Matrix<T>& otherMatrix) {
            if (!PossibleToPlusOrMinus(otherMatrix))
                throw runtime_error("Both matrices must have the same size");
            return (*PlusOrMinus(otherMatrix));
        }

        Matrix<T>& operator- (Matrix<T>& otherMatrix) {
            if (!PossibleToPlusOrMinus(otherMatrix))
                throw runtime_error("Both matrices must have the same size");
            return (*PlusOrMinus(otherMatrix, true));
        }

        Matrix<T>& operator* (Matrix<T>& otherMatrix) {
            if (!PossibleToMultiple(otherMatrix))
                throw runtime_error("The number of columns of the first matrix and number of rows of the second matrix must be equal");
            return (*Multiple(otherMatrix));
        }

        Matrix<T>& operator= (Matrix<T>& other) {
            ResizeWithValue(other.Rows(), other.Columns());
            for (int i = 0; i < other.Rows(); ++i) {
                for (int j = 0; j < other.Columns(); ++j) {
                    realMatrix[i][j] = other[{i, j}];
                }
            }
            return *this;
        }

        Matrix<T>& operator+= (Matrix<T>& otherMatrix) {
            if (!PossibleToPlusOrMinus(otherMatrix))
                throw runtime_error("Both matrices must have the same size");
            PlusOrMinusAndSetToFirst(*this, otherMatrix, false);
            return *this;
        }

        Matrix<T>& operator-= (Matrix<T>& otherMatrix) {
            if (!PossibleToPlusOrMinus(otherMatrix))
                throw runtime_error("Both matrices must have the same size");
            PlusOrMinusAndSetToFirst(*this, otherMatrix, true);
            return *this;
        }

        Matrix<T>& operator*= (Matrix<T>& otherMatrix) {
            if (!PossibleToMultiple(otherMatrix))
                throw runtime_error("The number of columns of the first matrix and number of rows of the second matrix must be equal");
            *this = (*Multiple(otherMatrix));
            return *this;
        }

        bool operator== (Matrix<T>& otherMatrix) {
            if (rows != otherMatrix.Rows() || columns != otherMatrix.Columns())
                return false;
            for (int i = 0; i < rows; ++i) {
                for (int j = 0; j < columns; ++j) {
                    if (*(*matrixLink)[i][j] != otherMatrix[{i, j}])
                        return false;
                }
            }
            return true;
        }

        ~Matrix() {
            for (auto v : matrix)
                delete[] & v;
            delete[] & matrix;

            for (auto v : transponentedMatrix)
                delete[] & v;
            delete[] & transponentedMatrix;

            for (auto v : realMatrix)
                delete[] & v;
            delete[] & realMatrix;

            delete matrixLink;
            delete transponentedMatrixLink;

            delete& rows;
            delete& columns;

        }

    private:

        vector<vector<T>> realMatrix;

        vector<vector<T*>> matrix;
        vector<vector<T*>> transponentedMatrix;

        vector<vector<T*>>* matrixLink;
        vector<vector<T*>>* transponentedMatrixLink;

        int rows;
        int columns;

        bool PossibleToMultiple(Matrix<T>& otherMatrix) {
            return (columns == otherMatrix.Rows());
        }

        bool PossibleToPlusOrMinus(Matrix<T>& otherMatrix) {
            return (rows == otherMatrix.Rows() && columns == otherMatrix.Columns());
        }

        Matrix<T>* PlusOrMinus(Matrix<T>& otherMatrix, bool minus = false) {
            Matrix<T>* temp = new Matrix<T>();
            temp = this;
            PlusOrMinusAndSetToFirst((*temp), otherMatrix, minus);
            return temp;
        }

        void PlusOrMinusAndSetToFirst(Matrix<T>& firstMatrix, Matrix<T>& secondMatrix, bool minus) {
            for (int i = 0; i < rows; ++i) {
                for (int j = 0; j < columns; ++j) {
                    firstMatrix[{i, j}] = minus ? firstMatrix[{i, j}] - secondMatrix[{i, j}] : firstMatrix[{i, j}] + secondMatrix[{i, j}];
                }
            }
        }

        Matrix<T>* Multiple(Matrix<T>& otherMatrix) {
            Matrix<T>* temp = new Matrix<T>();
            int newRows = rows;
            int newColumns = otherMatrix.Columns();
            temp->Resize(newRows, newColumns);

            otherMatrix.Transponent();

            for (int i = 0; i < newRows; ++i) {
                for (int j = 0; j < newColumns; ++j) {
                    (*temp)[{i, j}] = MultipleTwoVectors((*matrixLink)[i], otherMatrix[j]);
                }
            }

            otherMatrix.Transponent();

            return temp;
        }

        T MultipleTwoVectors(vector<T*>& firstVector, vector<T*>& secondVector) {
            T res = 0;
            for (int i = 0; i < firstVector.size(); ++i) {
                res += (*firstVector[i]) * (*secondVector[i]);
            }
            return res;
        }

        void TransponentInMemory() {
            if (rows < columns) {
                for (int i = 0; i < columns - rows; ++i) {
                    realMatrix.push_back(vector<T>(rows, 0));
                }
            }
            else {
                for (int i = 0; i < columns; ++i) {
                    for (int j = columns; j < rows; ++j) {
                        realMatrix[i].push_back(0);
                    }
                }
            }
            int newColumns;
            int newRows = newColumns = max(rows, columns);
            for (int i = 0; i < realMatrix.size(); ++i) {
                for (int j = i + 1; j < realMatrix[i].size(); ++j) {
                    swap(realMatrix[i][j], realMatrix[j][i]);
                }
            }
            if (rows < columns) {
                for (int i = 0; i < columns; ++i) {
                    for (int j = columns; j < rows; ++j) {
                        realMatrix[i].pop_back();
                    }
                }
            }
            else {
                for (int i = 0; i < columns - rows; ++i) {
                    realMatrix.pop_back();
                }
            }
            swap(rows, columns);
            ResizeLinks(rows, columns);
        }

        void ResizeWithValue(int rows, int columns, T value = 0) {
            this->rows = rows;
            this->columns = columns;
            realMatrix.assign(rows, vector<T>(columns, value));
            ResizeLinks(rows, columns);
        }

        void ResizeLinks(int rows, int columns) {
            matrix.resize(rows, vector<T*>(columns));
            transponentedMatrix.resize(columns, vector<T*>(rows));
            for (int i = 0; i < rows; ++i) {
                for (int j = 0; j < columns; ++j) {
                    matrix[i][j] = &realMatrix[i][j];
                    transponentedMatrix[j][i] = &realMatrix[i][j];
                }
            }
        }
    };

    template<typename T, class = enable_if_t<is_numeric_v<T>>>
    ostream& operator<<(ostream& out, Matrix<T>& m) {

        for (int i = 0; i < m.Rows(); ++i) {
            for (int j = 0; j < m.Columns(); ++j) {
                out << m[{i, j}] << " ";
            }
            out << "\n";
        }

        return out;
    }
}
#endif //MATRIX_H