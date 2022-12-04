#pragma once
#include "AntHill.h"

Anthill::Quenn* Anthill::Quenn::GetInstance()
{
	if (_instance == nullptr)
		_instance = new Quenn(Anthill::GetInstance()->Consumption[AntsEnum::quenn], Anthill::GetInstance()->Parameter[AntsEnum::quenn]);
	return _instance;
}

Anthill* Anthill::GetInstance()
{
	if (_instance == nullptr)
		_instance = new Anthill();
	return _instance;
}

Anthill* Anthill::_instance = nullptr;
Anthill* Anthill::Ant::anthill = nullptr;
Anthill::Quenn* Anthill::Quenn::_instance = nullptr;

Anthill::Ant::Ant()
{
	anthill = Anthill::GetInstance();
}
