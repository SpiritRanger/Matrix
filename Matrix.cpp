#include <iostream>
#include "Matrix.h"

using std::cout;

void TestMatrix();
void TestPlus();
void TestMinus();
void TestMultiple();
void TestSet();
void TestPlusAndSet();
void TestMinusAndSet();
void TestMultipleAndSet();
void TestEqual();
void TestTransponent();
void TestInmemoryTransponent();

int main() {

	TestMatrix();
	return 0;
}

void TestMatrix() {
	Matrix<int> m;
	m.Resize(2, 2);
	m[{0, 0}] = 0;
	m[{0, 1}] = 0;
	m[{1, 0}] = 0;
	m[{1, 1}] = 0;
	cout << "First matrix\n" << m;
	m.Assigne(2, 3, 1);
	Matrix<int> m2(2, 3, 2);
	cout << "Assigned first matrix\n" << m;
	cout << "Second matrix\n" << m2;
	Matrix<int> m3;
	m3 = m + m2;
	cout << "Summ of first matrix and second matrix\n" << m3;
	m3 = m - m2;
	cout << "Diff of first matrix and second matrix\n" << m3;
}

void TestPlus() {}
void TestMinus() {}
void TestMultiple() {}
void TestSet() {}
void TestPlusAndSet() {}
void TestMinusAndSet() {}
void TestMultipleAndSet() {}
void TestEqual() {}
void TestTransponent() {}
void TestInmemoryTransponent() {}