// Copyright (C) Team13. All rights reserved.

#include "ETVShipRepairShip.h"

AETVShipRepairShip::AETVShipRepairShip() : Super()
{
	Class = EETVShipClass::Repair;
}

void AETVShipRepairShip::Init(FString NewName, int32 MaxHP, int32 ShieldP, int32 NewShieldRechargeTime, int32 NewMoveRange, int32 Speed, int32 NewRepairPower)
{
	Super::Init(NewName, MaxHP, ShieldP, NewShieldRechargeTime, NewMoveRange, Speed);
	RepairPower = NewRepairPower;
}

void AETVShipRepairShip::InitRandom(FString NewName)
{
	Level = FMath::RandRange(50, 200);
	InitRandomWithLevel(NewName, Level);
}

void AETVShipRepairShip::InitRandomWithLevel(FString NewName, int32 PowerLvl)
{
	if (PowerLvl < 50)
		PowerLvl = 50;
	else if (PowerLvl > 200)
		PowerLvl = 200;

	Super::InitRandomWithLevel(NewName, PowerLvl);

	RepairPower = PowerLvl / 5;
}
