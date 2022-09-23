#pragma once
#ifndef TEST_H
#endif

extern "c" __decspec()

extern "C" __declspec(dllexport) double* gauss(double** a, double* y, int n);
extern "C" __declspec(dllexport) double* Matr(double** a, double* y, int n);
extern "C" __declspec(dllexport) void GetMatr(double** mas, double** p, int i, int j, int m);
extern "C" __declspec(dllexport) double Determinant(double** mas, int m);
extern "C" __declspec(dllexport) double* Kramer(double** x, double* fM, int m);

