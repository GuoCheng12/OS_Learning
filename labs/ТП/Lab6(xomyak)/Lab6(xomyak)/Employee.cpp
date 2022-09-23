#include "Employee.h"
#include <iostream>

using std::cout;
using std::endl;
using std::cin;

void Employee::countHours()
{
	cout << "Количество рабочих часов: ";
	cin >> countHoursInDay;
	cout << "Количество смен в месяц: ";
	cin >> countChange;
	hours = countHoursInDay * countChange;
}

void Employee::calculatePay()
{
	cout << "Оплата за час: ";
	cin >> payInHour;
	pay = payInHour * hours;
}

void Employee::getCountHours()
{
	cout << "Количество часов в общажитии: " << hours << endl;
}

void Employee::getPay()
{
	cout << "Заработная плата: " << pay << endl;
}