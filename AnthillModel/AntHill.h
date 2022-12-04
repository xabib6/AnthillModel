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
		cout << "����� ���������� ����� �����������. " << endl;
		SetInitValues();
		char input;
		int days;
		while (true)
		{
			cout << "=====================" << endl << endl;
			cout << "1)�������� ���������� � �����������. \n2)���������� ���. \n3)��������� ���������. " << endl;
			cin >> input;

			switch (input)
			{
			case '1':
				ShowInfo();
				break;
			case '2':
				cout << "������� ���� ����������? " << endl;
				cin >> days;
				for (int i = 0; i < days; i++)
				{
					GoNextDay();
					day++;
				}
				break;
			case '3':
				cout << "��������� ��������� �� " << day << " ���." << endl;
				return;
			default:
				cout <<"�������� ����." << endl;
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
			cout << "� ����������� ����������� ���. ��������� ��������� �� " << day << "���." << endl;
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
		cout << "���� " << day << endl;
		cout << "���������� ��� � ���������: " << TotalFood << endl;
		cout << "���������� ��������: " << ants.size() << endl;
		cout << "���������� �������: " << Quenn::GetInstance()->larvae.size() << endl;
		cout << "���������� ����������: " << PestCount << endl;
	}

	void SetInitValues()
	{		
		Consumption = new int[5];
		Parameter = new int[5];
		cout << "������� ����������� ���������: ";
		cin >> StorageCapacity;
		cout << "������� ��������� ������ ���: ";
		cin >> TotalFood;
		TotalFood = TotalFood > StorageCapacity ? StorageCapacity : TotalFood;
		cout << "������� ��������� ���������� ����������: ";
		cin >> PestCount;
		cout << "������� ������� ���������� � ����: ";
		cin >> PestPerDay;
		cout << "������� ���������� ���, ������������ ����� ����������: ";
		cin >> PestDamage;
		
		cout << "������� ����������� ���: \n������� - ";
		cin >> Consumption[AntsEnum::larve];
		cout << "������� - ";
		cin >> Consumption[AntsEnum::soldier];
		cout << "����������� - ";
		cin >> Consumption[AntsEnum::police];
		cout << "������� - ";
		cin >> Consumption[AntsEnum::worker];	
		cout << "�������� - ";
		cin >> Consumption[AntsEnum::quenn];
		cout << "������� ��������� ��������: \n������� - ���� �� ����������: ";
		cin >> Parameter[AntsEnum::larve];
		cout << "������� - ������������ ��������� : ";
		cin >> Parameter[AntsEnum::soldier];
		cout << "����������� - ������� ���������� ����: ";
		cin >> Parameter[AntsEnum::police];
		cout << "������� - ���������� ����: ";
		cin >> Parameter[AntsEnum::worker];
		cout << "�������� - ������������� �������: ";
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
