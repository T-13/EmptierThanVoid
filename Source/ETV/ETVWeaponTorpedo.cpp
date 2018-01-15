// Copyright (C) Team13. All rights reserved.

#include "ETVWeaponTorpedo.h"
#include "Kismet/KismetMathLibrary.h"

AETVWeaponTorpedo::AETVWeaponTorpedo() : Super()
{
	Type = AETVWeapon::DamageHull;
	FiringRange = 8;
}

void AETVWeaponTorpedo::Init(FName NewName, float MaximumHP, float Dmg, float WeaponWeight, float WeaponSize, int32 Range, int32 Speed)
{
	Super::Init(NewName, MaximumHP, Dmg, WeaponWeight, WeaponSize, Range);

	SquaresPerTurn = Speed;
}

void AETVWeaponTorpedo::InitRandom(FName NewName, int32 PowerLvl)
{
	Super::InitRandom(NewName, PowerLvl);

	if (PowerLvl < 50)
		PowerLvl = 50;
	else if (PowerLvl > 200)
		PowerLvl = 200;

	// Balancing the speed compared to power compared to lvl
	if (Damage > PowerLvl)
		SquaresPerTurn = FMath::RandRange(1, 3);
	else
		SquaresPerTurn = FMath::RandRange(2, 5);
}

int32 AETVWeaponTorpedo::GetSquaresPerTurn()
{
	return SquaresPerTurn;
}
