#pragma once
#include <iostream>
#include "AntHill.h"

using namespace std;
int main()
{
	setlocale(LC_ALL, "rus");
	srand(time(NULL));
	Anthill::GetInstance()->Simulate();
}
