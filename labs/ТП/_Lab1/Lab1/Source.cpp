#include <iostream>
#include <conio.h>
#include <string>
#include <Windows.h>

using namespace std;

const int NUMBER_FIELD = 2;

template <class T>
T** read_matrix(int n);

template <class T>
void write_matrix(T**, int);

template <class T>
struct LU {
	T** L;
	T** U;
};

LU <int> LU_matrix(int** table, int n);
LU <double> LU_matrix(double** table, int n);

void print_menu(string* menu, int n, int pos) {
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);

	SetConsoleTextAttribute(handle, FOREGROUND_GREEN);
	for (int i = 0; i < pos; i++) cout << menu[i] << endl;
		
	SetConsoleTextAttribute(handle, FOREGROUND_RED);
	cout << menu[pos] << endl;

	SetConsoleTextAttribute(handle, FOREGROUND_GREEN);
	for (int i = pos + 1; i < n; i++) cout << menu[i] << endl;
}

int main() {
	int choice;
	int pos = 0;
	string menu[NUMBER_FIELD] = { "1 - int" , "2 - double" };
	cout << "Calculation LU-matrix:\n";
	cout << "Choose the type (1 or 2)\n";
	print_menu(menu, NUMBER_FIELD, pos);
	char ch;
	int code;
	do {
		ch = getch();
		code = static_cast<int> (ch);
		switch (code)
		{
		case 80:
			if (pos < NUMBER_FIELD - 1) {
				system("cls");
				++pos;
				print_menu(menu, NUMBER_FIELD, pos);
			}
			break;
		
		case 72:
			if (pos > 0) {
				system("cls");
				--pos;
				print_menu(menu, NUMBER_FIELD, pos);
			}
			break;

		case 13:
			choice = ++pos;
			break;
		default:
			break;
		}
	} while (code != 13);
	
	int n;
	while (true) {
		cout << "\nEnter the size matrix\n";
		cin >> n;
		if (!cin && n > 0) {
			cout << "Wrong size entry. Repeat input\n";
			cin.clear();
			while (cin.get() != '\n');
		}
		else break;
	}
	cout << endl;

	int** table_i;
	double** table_d;
	int** L_i;
	double** L_d;
	int** U_i;
	double** U_d;

	switch (choice)
	{
	case 1:
		table_i = read_matrix <int>(n);
		LU <int> temp_i = LU_matrix(table_i, n);
		L_i = temp_i.L;
		U_i = temp_i.U;
		break;
	case 2:
		table_d = read_matrix <double>(n);
		LU <double> temp_d = LU_matrix(table_d, n);
		L_d = temp_d.L;
		U_d = temp_d.U;
		break;
	default:
		return 0;
	}
		

	switch (choice)
	{
	case 1:
		cout << "\nCurrent matrix:\n";
		write_matrix <int>(table_i, n);
		cout << "\nMatrix L: \n";
		write_matrix <int>(L_i, n);
		cout << "\nMatrix U: \n";
		write_matrix <int>(U_i, n);
		for (int i = 0; i < n; i++) {
			delete[] table_i[i];
			delete[] L_i[i];
			delete[] U_i[i];
		}
		delete[] table_i;
		delete[] L_i;
		delete[] U_i;
		
		break;
	case 2:
		cout << "\nCurrent matrix:\n";
		write_matrix <double>(table_d, n);
		cout << "\nMatrix L: \n";
		write_matrix <double>(L_d, n);
		cout << "\nMatrix U: \n";
		write_matrix <double>(U_d, n);
		
		for (int i = 0; i < n; i++) {
			delete[] table_d[i];
			delete[] L_d[i];
			delete[] U_d[i];
		}
		delete[] table_d;
		delete[] L_d;
		delete[] U_d;

		break;
	default:
		return 0;
	}

	_getch();
	return 0;
}

template <class T>
T** read_matrix(int n) {
	T** table = new T*[n];
	for (int i = 0; i < n; i++) {
		table[i] = new T[n];
		for (int j = 0; j < n; j++) {
			while (true) {
				cout << "A[" << i << "] [" << j << "]: ";
				cin >> table[i][j]; 
				if (!cin || (table[i][j] == 0 && i == j)) {
					cout << "Wrong value of the matrix element. Repeat input\n";
					cin.clear();
					while (cin.get() != '\n');
				}
				else break;
			}
		}
	}
	return table;
}

template <class T>
void write_matrix(T** table, int n) {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++)
			cout << table[i][j] << "\t";
		cout << endl;
	}
	cout << endl;
}

LU <int> LU_matrix(int** table, int n) {
	LU <int> temp;

	temp.U = new int*[n];
	for (int i = 0; i < n; i++) {
		temp.U[i] = new int[n];
		for (int j = 0; j < n; j++)
			temp.U[i][j] = table[i][j];
	}
	
	temp.L = new int*[n];
	for (int i = 0; i < n; i++) {
		temp.L[i] = new int[n];
		for (int j = 0; j < n; j++)
			temp.L[i][j] = 0;
	}
	
	for (int i = 0; i < n; i++)
		for (int j = i; j < n; j++)
			temp.L[j][i] = temp.U[j][i] / temp.U[i][i];
	
	for (int k = 1; k < n; k++) {
		for (int i = k - 1; i < n; i++)
			for (int j = i; j < n; j++)
				temp.L[j][i] = temp.U[j][i] / temp.U[i][i];

		for (int i = k; i < n; i++)
			for (int j = k - 1; j < n; j++)
				temp.U[i][j] -= temp.L[i][k - 1] * temp.U[k - 1][j];
	}
	return temp;
}

LU <double> LU_matrix(double** table, int n) {
	LU <double> temp;

	temp.U = new double*[n];
	for (int i = 0; i < n; i++) {
		temp.U[i] = new double[n];
		for (int j = 0; j < n; j++)
			temp.U[i][j] = table[i][j];
	}

	temp.L = new double*[n];
	for (int i = 0; i < n; i++) {
		temp.L[i] = new double[n];
		for (int j = 0; j < n; j++)
			temp.L[i][j] = 0;
	}

	for (int i = 0; i < n; i++)
		for (int j = i; j < n; j++)
			temp.L[j][i] = temp.U[j][i] / temp.U[i][i];

	for (int k = 1; k < n; k++) {
		for (int i = k - 1; i < n; i++)
			for (int j = i; j < n; j++)
				temp.L[j][i] = temp.U[j][i] / temp.U[i][i];

		for (int i = k; i < n; i++)
			for (int j = k - 1; j < n; j++)
				temp.U[i][j] = temp.U[i][j] - temp.L[i][k - 1] * temp.U[k - 1][j];
	}
	return temp;
}