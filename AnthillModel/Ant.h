#pragma once
#include <queue>
#include "AntHill.h"
using namespace std;
class Ant
{
protected:
	int _consumption;

public:
	virtual void Impact() = 0;
};
//Муравейник.Типы объектов : матка, солдаты,
//полицейские, рабочие, личинки, вредители.Каждый представитель
//является отдельным экземпляром класса.Муравейник имеет склад
//пищи фиксированного размера.Матка создает личинки, из
//личинок выводятся солдаты, полицейские или рабочие муравьи и
//ест очень много, солдаты уменьшают число вредителей и много
//едят, полицейские помогают увеличить число добываемой пищи и
//едят меньше солдат, рабочие добывают пищу и едят меньше
//полицейских, личинки только едят.Матка имеет следующие
//параметры :
// кол - во потребляемой пищи;
// кол - во рождаемых личинок.
//Солдаты имеют следующие параметры
// кол - во потребляемой пищи;
// кол - во уничтожаемых вредителей.
//Полицейские имеются следующие параметры :
// кол - во потребляемой пищи;
// процент, на который увеличивается запас пищи.
//Рабочие имеют следующие параметры :
// кол - во потребляемой пищи;
// кол - во добываемой пищи.
//Личинки имеют только один параметр : кол - во потребляемой
//пищи.Через некоторое время личинки должны разродиться в
//новую форму.При изменении формы личинки должны об этом
//сигнализировать матку, чтобы она определила тип, в который
//превратится личинка.Показать работу муравейника для различных
//значений числа муравьев, вредителей, пищи.

class Larve : public Ant
{
public:
	int _daysBeforeHatch;
	Larve(int daysBeforeHatch, int consumption)
	{
		_daysBeforeHatch = daysBeforeHatch;
		_consumption = consumption;
	}
	void Impact()
	{
		Anthill::GetInstance()->TotalFood -= _consumption;
	}
};

class Quenn : Ant
{
private:
	static Quenn* _instance;
	int _larvaePerDay, _daysBeforeHatch;


public:
	static Quenn* GetInstance()
	{
		if (_instance == nullptr)
			_instance = new Quenn;
		return _instance;
	}
	Ant* SelectType(queue<Larve> &larvae, queue<Ant*> ants, int* Consumption, int* Parameter)
	{
		for (int i = 0; i < larvae.size(); i++)
		{
			if (larvae.front()._daysBeforeHatch == 0)
			{
				//Запушить в очередь муравьёв рандомного муравья
				larvae.pop();
			}
			else
				break;
		}
	}
	void LayLarvae(queue<Larve>& larvae, int larveConsumption)
	{
		for (int i = 0; i < _larvaePerDay; i++)
		{
			Larve larve(_daysBeforeHatch, larveConsumption);
			larvae.push(larve);
		}
	}
	void Impact()
	{
		Anthill::GetInstance()->TotalFood -= _consumption;
	}

};

class Soldier : Ant
{
private:
	int _killedPest;

public:
	Soldier(int consumption, int killedPest)
	{
		_consumption = consumption;
		_killedPest = killedPest;
	}
	void Impact()
	{
		Anthill::GetInstance()->TotalFood -= _consumption;
		Anthill::GetInstance()->PestCount -= _killedPest;
	}
};
class Police : Ant
{
private:
	int _increasePercent;

public:
	Police(int consumption, int increasePercent)
	{
		_consumption = consumption;
		_increasePercent = increasePercent;
	}
	void Impact()
	{
		float* totalFood = &Anthill::GetInstance()->TotalFood;
		float* storageCapacity = &Anthill::GetInstance()->StorageCapacity;
		*totalFood -= _consumption;
		
		float foodIncrease = *totalFood / 100 * _increasePercent;
		*totalFood = *totalFood + foodIncrease <= *storageCapacity ? *totalFood + foodIncrease : *storageCapacity;
	}
};

class Worker : Ant
{
private:
	int _foodProduction;

public:
	Worker(int consumption, int foodProduction)
	{
		_consumption = consumption;
		_foodProduction = foodProduction;
	}
	void Impact()
	{
		float* totalFood = &Anthill::GetInstance()->TotalFood;
		float* storageCapacity = &Anthill::GetInstance()->StorageCapacity;
		*totalFood -= _consumption;
		*totalFood = *totalFood + _foodProduction <= *storageCapacity ? *totalFood + _foodProduction : *storageCapacity;
	}
};
enum Consumption
{
	larve = 0,
	soldier = 1,
	police = 2,
	worker = 3
};
