// Copyright (C) Team13. All rights reserved.

#include "ETVWeapon.h"

AETVWeapon::AETVWeapon() : Super()
{
}

void AETVWeapon::Fire(int32 x, int32 y)
{
}

TArray<int32> AETVWeapon::GetPossibleTargets()
{
    return TArray<int32>();
}

int32 AETVWeapon::GetSlotSpecifications()
{
    return int32();
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
