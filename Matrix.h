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

template<typename T>
constexpr bool is_numeric_v =
is_same<T, int>::value
|| is_same<T, long double>::value
|| is_same<T, short>::value
|| is_same<T, long long>::value
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

    void TransponentMatrix() {
        swap(matrixLink, transponentedMatrixLink);
        swap(rows, columns);
    }

    int Rows() {
        return rows;
    }

    int Columns() {
        return columns;
    }

    T& operator[](pair<size_t, size_t> i) {
        if (i.first < 0 || i.first >= rows || i.second < 0 || i.second >= columns)
            throw runtime_error("Matrix subscript out of range");

        return *(*matrixLink)[i.first][i.second];
    }

    friend ostream& operator<<(ostream& out, const Matrix<T>& m);

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

    void ResizeWithValue(int rows, int columns, T value = 0) {
        this->rows = rows;
        this->columns = columns;
        realMatrix.assign(rows, vector<T>(columns, value));
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