#pragma once
#include <queue>
#include <iostream>
using namespace std;

class Anthill
{	

private:
	static Anthill* _instance;	

	Anthill(){}
	
	class Ant
	{
	protected:
		int _consumption;
		static Anthill* anthill;
	public:
		virtual void Impact() = 0;
		Ant();
	};

	class Larve : public Ant
	{
	public:
		int _daysBeforeHatch;
		Larve(int daysBeforeHatch, int consumption)
		{
			_daysBeforeHatch = daysBeforeHatch;
			_consumption = consumption;
		}
		void Impact() override
		{
			anthill->TotalFood -= _consumption;
			_daysBeforeHatch--;
		}
	};

	class Quenn : public Ant
	{
	private:
		static Quenn* _instance;
		int _larvaePerDay;

		Quenn(int consumption, int larvaePerDay)
		{
			_consumption = consumption;
			_larvaePerDay = larvaePerDay;
		}

	public:
		vector <Larve> larvae;

		static Quenn* GetInstance();

		void SelectType()
		{
			Ant* newAnt;
			int* Consumption = anthill->Consumption;
			int* Parameter = anthill->Parameter;
			{
				for (int i = 0; i < larvae.size(); i++)
				{
					if (larvae.front()._daysBeforeHatch == 0)
					{
						int Rand = rand() % 3;
						AntsEnum ant;
						switch (Rand)
						{
						case 0:
							ant = soldier;
							newAnt = new Soldier(Consumption[ant], Parameter[ant]);
							break;
						case 1:
							ant = police;
							newAnt = new Police(Consumption[ant], Parameter[ant]);
							break;
						case 2:
							ant = worker;
							newAnt = new Worker(Consumption[ant], Parameter[ant]);
							break;
						default:
							break;
						}
						anthill->ants.push(newAnt);
						larvae.erase(larvae.begin());
					}
					else
						break;
				}

			}
		}
		void LayLarvae()
		{
			for (int i = 0; i < _larvaePerDay; i++)
			{
				Larve larve(anthill->Parameter[AntsEnum::larve], anthill->Consumption[AntsEnum::larve]);
				larvae.push_back(larve);
			}
		}
		void Impact() override
		{
			anthill->TotalFood -= _consumption;
			LayLarvae();
			SelectType();
		}

	};

	class Soldier : public Ant
	{
	private:
		int _killedPest;

	public:
		Soldier(int consumption, int killedPest)
		{
			_consumption = consumption;
			_killedPest = killedPest;
		}
		void Impact() override
		{
			anthill->TotalFood -= _consumption;
			auto& pestCount = anthill->PestCount;
			pestCount = pestCount - _killedPest < 0 ? 0 : pestCount - _killedPest;
		}
	};

	class Police : public Ant
	{
	private:
		int _increasePercent;

	public:
		Police(int consumption, int increasePercent)
		{
			_consumption = consumption;
			_increasePercent = increasePercent;
		}
		void Impact() override
		{
			float& totalFood = anthill->TotalFood;
			float& storageCapacity = anthill->StorageCapacity;
			totalFood -= _consumption;

			float foodIncrease = totalFood / 100 * _increasePercent;
			totalFood = totalFood + foodIncrease <= storageCapacity ? totalFood + foodIncrease : storageCapacity;
		}
	};

	class Worker : public Ant
	{
	private:
		int _foodProduction;

	public:
		Worker(int consumption, int foodProduction)
		{
			_consumption = consumption;
			_foodProduction = foodProduction;
		}
		void Impact() override
		{
			float& totalFood = anthill->TotalFood;
			float& storageCapacity = anthill->StorageCapacity;

			totalFood -= _consumption;
			totalFood = totalFood + _foodProduction <= storageCapacity ? totalFood + _foodProduction : storageCapacity;
		}
	};

public:
	int* Consumption, * Parameter;
	int day;	
	static Anthill* GetInstance();

	queue<Ant*> ants;

	



	float TotalFood, StorageCapacity;
	int PestCount, PestPerDay, PestDamage;

	void Simulate()
	{
		cout << "Здесь начинается жизнь муравейника. " << endl;
		SetInitValues();
		char input;
		int days;
		while (true)
		{
			cout << "=====================" << endl << endl;
			cout << "1)Показать информацию о муравейнике. \n2)Пропустить дни. \n3)Завершить симуляцию. " << endl;
			cin >> input;

			switch (input)
			{
			case '1':
				ShowInfo();
				break;
			case '2':
				cout << "Сколько дней пропустить? " << endl;
				cin >> days;
				for (int i = 0; i < days; i++)
				{
					GoNextDay();
					day++;
				}
				break;
			case '3':
				cout << "Симуляция завершена на " << day << " дне." << endl;
				return;
			default:
				cout <<"Неверный ввод." << endl;
				continue;
			}			
			cout << "=====================" << endl << endl;
		}
	}


	void GoNextDay()
	{
		PestCount += PestPerDay;
		TotalFood -= PestCount * PestDamage;
		if (TotalFood <= 0)
		{
			cout << "В муравейнике закончилась еда. Симуляция завершена на " << day << "дне." << endl;
			return;
		}
		auto* newAnts = &ants._Get_container();		
		for (int i = 0; i < ants.size(); i++)
		{
			newAnts->at(i)->Impact();			
		}
		for (int i = 0; i < Quenn::GetInstance()->larvae.size(); i++)
		{
			Quenn::GetInstance()->larvae.at(i).Impact();
		}
	}	

	void ShowInfo()
	{
		cout << "День " << day << endl;
		cout << "Количество еды в хранилище: " << TotalFood << endl;
		cout << "Количество муравьёв: " << ants.size() << endl;
		cout << "Количество личинок: " << Quenn::GetInstance()->larvae.size() << endl;
		cout << "Количество вредителей: " << PestCount << endl;
	}

	void SetInitValues()
	{		
		Consumption = new int[5];
		Parameter = new int[5];
		cout << "Введите вместимость хранилища: ";
		cin >> StorageCapacity;
		cout << "Введите начальные запасы еды: ";
		cin >> TotalFood;
		TotalFood = TotalFood > StorageCapacity ? StorageCapacity : TotalFood;
		cout << "Введите начальное количество вредителей: ";
		cin >> PestCount;
		cout << "Введите прирост вредителей в день: ";
		cin >> PestPerDay;
		cout << "Введите количество еды, уничтожаемое одним вредителем: ";
		cin >> PestDamage;
		
		cout << "Введите потребление еды: \nЛичинки - ";
		cin >> Consumption[AntsEnum::larve];
		cout << "Солдаты - ";
		cin >> Consumption[AntsEnum::soldier];
		cout << "Полицейские - ";
		cin >> Consumption[AntsEnum::police];
		cout << "Рабочие - ";
		cin >> Consumption[AntsEnum::worker];	
		cout << "Королева - ";
		cin >> Consumption[AntsEnum::quenn];
		cout << "Введите параметры муравьёв: \nЛичинки - дней до вылупления: ";
		cin >> Parameter[AntsEnum::larve];
		cout << "Солдаты - уничтожаемые вредители : ";
		cin >> Parameter[AntsEnum::soldier];
		cout << "Полицейские - процент увеличения пищи: ";
		cin >> Parameter[AntsEnum::police];
		cout << "Рабочие - добываемая пища: ";
		cin >> Parameter[AntsEnum::worker];
		cout << "Королева - откладываемые личинки: ";
		cin >> Parameter[AntsEnum::quenn];

		Quenn* quenn = Quenn::GetInstance();
		ants.push(quenn);
	}
	enum AntsEnum
	{
		larve = 0,
		soldier = 1,
		police = 2,
		worker = 3,
		quenn = 4
	};
};
