#pragma once
#include <iostream>

using std::cout;
using std::endl;

enum GENDER { F, M };

class LivingDorm {
protected:
	int age;
	GENDER gender;

public:
	LivingDorm() { age = 15; gender = F; }
	void setData(int a, GENDER g) { age = a; gender = g; }
	void getData() {
		cout << "Возраст: " << age << endl;
		cout << "Пол(F/M): " << (gender == GENDER::F ? "F" : "M") << endl;
	}
	virtual void countHours() = 0;
	virtual void calculatePay() = 0;
};