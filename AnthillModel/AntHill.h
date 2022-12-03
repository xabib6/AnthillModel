#pragma once
#include "Ant.h"
#include <iostream>
using namespace std;

class Anthill
{
private:
	static Anthill* _instance;

public:
	static Anthill* GetInstance()
	{
		if (_instance == nullptr)
			_instance = new Anthill();
		return _instance;
	}
	float TotalFood;
	int PestCount;
	int PestPerDay;
	float StorageCapacity;

	int *Consumption, *Parameter;

	void SetInitValues()
	{
		cin >> StorageCapacity;
		cin >> PestPerDay;
		cin >> PestCount;
		cin >> TotalFood;
		cin >> Consumption[larve];
		cin >> Consumption[soldier];
		cin >> Consumption[police];
		cin >> Consumption[worker];
	}
};