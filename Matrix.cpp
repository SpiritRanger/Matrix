#include <iostream>
#include "Matrix.h"

using std::cout;

void TestMatrix();

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

	cout << m;

	m.Assigne(2, 3, 1);

	cout << m;

	m.TransponentMatrix();

	cout << m;
}