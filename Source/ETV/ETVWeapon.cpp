// Copyright (C) Team13. All rights reserved.

#include "ETVWeapon.h"
#include "Kismet/KismetMathLibrary.h"

AETVWeapon::AETVWeapon() : Super()
{
}

void AETVWeapon::Init(FName NewName, float MaximumHP, float Dmg, float WeaponWeight, float WeaponSize, int32 Range)
{
	Name = NewName;
	MaximumHealthPoints = MaximumHP;
	HealthPoints = MaximumHealthPoints;
	Damage = Dmg;
	Size = WeaponSize;
	FiringRange = Range;
}

void AETVWeapon::InitRandom(FName NewName, int32 PowerLvl)
{
	if (PowerLvl < 50)
		PowerLvl = 50;
	else if (PowerLvl > 200)
		PowerLvl = 200;

	// It should take an avarage of 10 shots to kill a ship of similiar lvl
	int32 AvgShotsToKill = 10;

	// The range of values we can generate
	int32 DMGRangeMin = PowerLvl - 25;
	int32 DMGRangeMax = PowerLvl + 25;

	int32 ReqRangeMin = PowerLvl / 10;
	int32 ReqRangeMax = ReqRangeMin * 2;

	Name = NewName;

	MaximumHealthPoints = FMath::RandRange(DMGRangeMin, DMGRangeMax);
	HealthPoints = MaximumHealthPoints;
	Damage = FMath::RoundToInt(FMath::RandRange(DMGRangeMin, DMGRangeMax) / AvgShotsToKill);

	Size = FMath::RandRange(ReqRangeMin, ReqRangeMax);
	Weight = FMath::RandRange(ReqRangeMin, ReqRangeMax);
}

/* This will be added later
TArray<int32> AETVWeapon::GetAngle()
{
	return Angle;
}*/

float AETVWeapon::GetWeight()
{
	return Weight;
}


float AETVWeapon::GetSize()
{
	return Size;
}

int32 AETVWeapon::GetReport()
{
	return int32();
}

int32 AETVWeapon::GetType()
{
	return Type;
}

float AETVWeapon::GetDMG()
{
	return Damage;
}

void AETVWeapon::SetHP(int32 newValue)
{
	if (newValue >= 0 || newValue <= MaximumHealthPoints)
		HealthPoints = newValue;
	else if (newValue < 0)
		HealthPoints = 0;
	else if (newValue > MaximumHealthPoints)
		HealthPoints = MaximumHealthPoints;
}

int32 AETVWeapon::GetHP()
{
	return HealthPoints;
}
