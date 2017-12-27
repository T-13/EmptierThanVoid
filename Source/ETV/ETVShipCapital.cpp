// Copyright (C) Team13. All rights reserved.

#include "ETVShipCapital.h"


AETVShipCapital::AETVShipCapital() : Super()
{
	Class = EETVShipClass::Capital;
}

void AETVShipCapital::Init(FString NewName, int32 MaxHP, int32 ShieldP, int32 NewShieldRechargeTime, int32 NewMoveRange, int32 Speed, bool HyperDrive, float NewLaserResistance, int32 Hangars)
{
	Super::Init(NewName, MaxHP, ShieldP, NewShieldRechargeTime, NewMoveRange, Speed);
	HasHyperdrive = HyperDrive;
	LaserResistance = NewLaserResistance;
	NumOfHangars = Hangars;
}

void AETVShipCapital::InitRandom(FString NewName)
{
	Level = FMath::RandRange(125, 225);
	InitRandomWithLevel(NewName, Level);
}

void AETVShipCapital::InitRandomWithLevel(FString NewName, int32 PowerLvl)
{
	// Bonus because it is the Capital Ship
	PowerLvl += 25;

	if (PowerLvl < 50)
		PowerLvl = 50;
	else if (PowerLvl > 200)
		PowerLvl = 200;

	Super::InitRandomWithLevel(NewName, PowerLvl);

	// Hyperdrive is more likely to be present in higher level Capital Ships
	float chance = FMath::RandRange(PowerLvl / 200, 1);
	if (chance > 0.85)
		HasHyperdrive = true;

	LaserResistance = FMath::RandRange(PowerLvl / 200, 1);

	// The better the level the lower the time (min 1)
	int32 MinHangars = 200 / PowerLvl;
	int32 MaxHangars = MinHangars + 3;
	NumOfHangars = FMath::RandRange(MinHangars, MaxHangars);
}
