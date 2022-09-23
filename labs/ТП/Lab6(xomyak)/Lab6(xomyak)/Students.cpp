#include "Students.h"
#include <iostream>

using std::cout;
using std::endl;
using std::cin;

void Students::countHours()
{
	cout << "Количество пар в день: ";
	cin >> countClasses;
	cout << "Количество учебных дней: ";
	cin >> countDays;
	hours = 24 * 31 - 1.5 * countClasses * countDays;
}

void Students::calculatePay()
{
	cout << "Оплата за час: ";
	cin >> payInHour;
	pay = payInHour * hours;
}

void Students::getCountHours()
{
	cout << "Количество часов в общажитии: " << hours << endl;
}

void Students::getPay()
{
	cout << "Плата за общажитие: " << pay << endl;
}
