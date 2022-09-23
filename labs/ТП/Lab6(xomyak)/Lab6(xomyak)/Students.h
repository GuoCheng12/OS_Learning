#pragma once
#include "LivingDorm.h"

class Students : public LivingDorm
{
private:
	int hours;
	int pay;
	int countClasses;
	int countDays;
	int payInHour;
public:
	Students() { hours = 0; pay = 0; }
	void countHours();
	void calculatePay();
	void getCountHours();
	void getPay();
};

