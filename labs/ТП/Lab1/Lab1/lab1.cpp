#include "stdio.h"
#include "pch.h"
#include <iostream>
#include <string>
#include <Windows.h>
#include <conio.h>


using namespace std;
const int num = 2;

template <class T> //шаблон для ввода матрицы
T** read_matrix(int n, int m);

template <class T> //шаблон для вывода матрицы
void write_matrix(T**, int);

//перегруженные функции для разности матриц
int **razn(int **mas1_i, int **mas2_i, int n, int m);
float **razn(float **mas1_f, float **mas2_f, int n, int m);

float** mas1_f;
int** mas1_i;
float** mas2_f;
int** mas2_i;
float** mas3_f;
int** mas3_i;


void print_menu(string* menu, int n, int pos) {
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);

	SetConsoleTextAttribute(handle, FOREGROUND_GREEN);
	for (int i = 0; i < pos; i++) cout << menu[i] << endl;

	SetConsoleTextAttribute(handle, FOREGROUND_RED);
	cout << menu[pos] << endl;

	SetConsoleTextAttribute(handle, FOREGROUND_GREEN);
	for (int i = pos + 1; i < n; i++) cout << menu[i] << endl;
}


int **razn(int **mas1_i, int **mas2_i, int n, int m)
{
	mas3_i = new int*[n];
	for (int i = 0; i < n; i++)
	{
		mas3_i[i] = new int [n];
		for (int j = 0; j < m; j++)
		{
			mas3_i[i][j] = mas1_i[i][j] - mas2_i[i][j];
		}
	}
	return mas3_i;
}

float **razn(float **mas1_f, float **mas2_f, int n, int m)
{
	mas3_f = new float* [n];
	for (int i = 0; i < n; i++)
	{
		mas3_f[i] = new float [m];
		for (int j = 0; j < m; j++)
		{
			mas3_f[i][j] = mas1_f[i][j] - mas2_f[i][j];
		}
	}
	return mas3_f;
}

template <class T>
T** read_matrix(int n, int m) {
	T** table = new T*[n];
	for (int i = 0; i < n; i++) {
		table[i] = new T[m];
		for (int j = 0; j < m; j++) {
			while (true) {
				cout << "A[" << i << "] [" << j << "]: ";
				cin >> table[i][j];
				if (!cin || (table[i][j] == 0 && i == j)) {
					cout << "Некорректный ввод. Повторите попытку\n";
					cin.clear();
					while (cin.get() != '\n');
				}
				else break;
			}
		}
	}
	return table;
}

/*for (int i = 0; i < n; i++)
{
	for (int j = 0; j < m; j++)
	{
		printf("Введите элемент mas[%d] [%d] 1 матрицы:", i, j);
		while (!(cin >> mas[i][j]) || (cin.peek() != '\n'))
		{
			cin.clear();
			while (cin.get() != '\n');
			printf("Некорректный ввод, введите mas[%d] [%d] 1 матрицы:", i, j);
		}
	}
}*/
template <class T>
void write_matrix(T** tabl, int n, int m) 
{
	for (int i = 0; i < n; i++) 
	{
		for (int j = 0; j < m; j++)
			cout << tabl[i][j] << "\t";
		cout << endl;
	}
	cout << endl;
}

int main()
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(handle, FOREGROUND_INTENSITY);
	setlocale(LC_ALL, "ru");
	int choice;
	int pos = 0;
	string menu[num] = { "1 - int" , "2 - float" };
	cout << "Нахождение разности матриц\n";
	cout << "Выберите тип данных (1 или 2)\n";
	print_menu(menu, num, pos);
	char ch;
	int code;
	do {
		ch = _getch();
		code = static_cast<int> (ch);
		switch (code)
		{
		case 80:
			if (pos < num - 1) {
				system("cls");
				++pos;
				print_menu(menu, num, pos);
			}
			break;

		case 72:
			if (pos > 0) {
				system("cls");
				--pos;
				print_menu(menu, num, pos);
			}
			break;

		case 13:
			choice = ++pos;
			break;
		default:
			break;
		}
	} while (code != 13);

	
	int n = 0; int m = 0;
	cout << "Введите кол-во строк матрицы:";
	while (!(cin >> n) || (cin.peek() != '\n')|| n < 10 || n > 100)
	{
		cin.clear();
		while (cin.get() != '\n');
		cout << "Некорректный ввод, введите кол-во строк снова:" << endl;
	}
	cout << "Введите кол-во столбцов матрицы:";
	while (!(cin >> m) || (cin.peek() != '\n') || m < 10 || m > 100)
	{
		cin.clear();
		while (cin.get() != '\n');
		cout << "Некорректный ввод, введите кол-во столбцов снова:" << endl;
	}

	

	switch (choice)
	{
	case 1:
		mas1_i = read_matrix <int>(n,m);
		mas2_i = read_matrix <int>(n,m);
		razn(mas1_i, mas2_i, n, m);
		cout << "\n1 матрица\n";
		write_matrix <int>(mas1_i, n, m);
		cout << "\n2 матрица\n";
		write_matrix <int>(mas2_i, n, m);
		cout << "\nрезультат\n";
		write_matrix <int>(mas3_i, n, m);
		for (int i = 0; i < n; i++) {
			delete[] mas3_i[i];
			delete[] mas1_i[i];
			delete[] mas2_i[i];
		}
		delete[] mas1_i;
		delete[] mas2_i;
		delete[] mas3_i;
		break;
	case 2:
		mas1_f = read_matrix <float>(n, m);
		mas2_f = read_matrix <float>(n, m);
		razn(mas1_f, mas2_f, n, m);
		cout << "\n1 матрица\n";
		write_matrix <float>(mas1_f, n, m);
		cout << "\n2 матрица\n";
		write_matrix <float>(mas2_f, n, m);
		cout << "\nрезультат\n";
		write_matrix <float>(mas3_f, n, m);
		for (int i = 0; i < n; i++) {
			delete[] mas3_f[i];
			delete[] mas1_f[i];
			delete[] mas2_f[i];
		}
		delete[] mas1_f;
		delete[] mas2_f;
		delete[] mas3_f;
		break;
	default:
		return 0;
	}
	system("pause");
	//_getch();
	return 0;

	//bool ok;
	//ok = false;
	/*while (ok == false)
	{
		cout << "Введите кол-во строк 1 матрицы:";
		while (!(cin >> n) || (cin.peek() != '\n') || n < 10 || n > 100)
		{
			cin.clear();
			while (cin.get() != '\n');
			cout << "Некорректный ввод, введите кол-во строк снова:" << endl;
		}
		cout << "Введите кол-во столбцов 1 матрицы:";
		while (!(cin >> m) || (cin.peek() != '\n') || m < 10 || m > 100)
		{
			cin.clear();
			while (cin.get() != '\n');
			cout << "Некорректный ввод, введите кол-во столбцов снова:" << endl;
		}
		/*cout << "Введите кол-во строк 2 матрицы:";
		while (!(cin >> n2) || (cin.peek() != '\n') || n2 < 10 || n2 > 100)
		{
			cin.clear();
			while (cin.get() != '\n');
			cout << "Некорректный ввод, введите кол-во строк снова:" << endl;
		}
		cout << "Введите кол-во столбцов 2 матрицы:";
		while (!(cin >> m2) || (cin.peek() != '\n') || m2 < 10 || m2 > 100)
		{
			cin.clear();
			while (cin.get() != '\n');
			cout << "Некорректный ввод, введите кол-во строк снова:" << endl;
		}
		if (n != n2 || m != m2)
		{
			cout << "Размерность матриц не совпадает, введите матрицы заного";
			n = 0;
			n2 = 0;
			m = 0;
			m2 = 0;
		}
		else ok = true;
	}*/
	
	/*for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
		{
			printf("Введите элемент mas[%d] [%d] 2 матрицы:", i, j);
			while (!(cin >> mas2[i][j]) || (cin.peek() != '\n'))
			{
				cin.clear();
				while (cin.get() != '\n');
				printf("Некорректный ввод, введите mas[%d] [%d] 1 матрицы:", i, j);
			}
		}
	}*/
	
	/*system("cls");
	cout << "\n1 матрица\n";
	write_matrix <class T>(table, n);*/


}