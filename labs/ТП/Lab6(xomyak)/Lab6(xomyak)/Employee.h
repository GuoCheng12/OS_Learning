#pragma once
#include "LivingDorm.h"
class Employee : public LivingDorm
{
private:
	int countHoursInDay;
	int countChange;
	int payInHour;
	int hours;
	int pay;
public:
	void countHours();
	void calculatePay();
	void getCountHours();
	void getPay();
};

