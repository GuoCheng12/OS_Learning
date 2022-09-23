#include <iostream>
#include "LivingDorm.h"
#include "Students.h"
#include "Employee.h"
#include <string>

using namespace std;

struct livingDorm {
	LivingDorm* living;
	bool isStudent;
};

int main() {
	livingDorm living[100];
	setlocale(LC_ALL, "rus");

	int n = 0;
	string choice;
	do {
		cout << "\nСтудент (s) или Работник (e): ";
		cin >> choice;
		if (choice.length() == 1 && (choice == "e" || choice == "s")) {
			cout << "Введите возраст: ";
			int age;
			cin >> age;
			cout << "Введите пол(F/M): ";
			GENDER gender;
			string temp;
			cin >> temp;
			if (temp == "F")
				gender = GENDER::F;
			else if (temp == "M")
				gender = GENDER::M;
			else {
				cout << "Повторите ввод";
				continue;
			}

			if (choice == "s") {
				living[n].living = new Students;
				living[n].isStudent = true;
			}
			else {
				living[n].living = new Employee;
				living[n].isStudent = false;
			}

			living[n].living->setData(age, gender);
			living[n].living->countHours();
			living[n].living->calculatePay();
			n++;
			if (n == 100) return 0;
		}
		else cout << "Неизвестная команда\n";
		cout << "Продолжить ввод? (y/n)? ";
		cin >> choice;
	} while (choice == "y");

	cout << endl;
	for (int j = 0; j < n; j++)
	{
		living[j].living->getData();
		if (living[j].isStudent) {
			((Students*)living[j].living)->getCountHours();
			((Students*)living[j].living)->getPay();
		}
		else {
			((Employee*)living[j].living)->getCountHours();
			((Employee*)living[j].living)->getPay();
		}
		cout << endl;
	}

	system("pause");
}