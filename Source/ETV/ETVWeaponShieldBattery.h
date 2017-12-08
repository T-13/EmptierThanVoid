// Copyright (C) Team13. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "ETVWeapon.h"
#include "ETVWeaponShieldBattery.generated.h"

/**
 * Heald Shield of friendly ships
 */
UCLASS()
class ETV_API AETVWeaponShieldBattery : public AETVWeapon
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AETVWeaponShieldBattery();
	// Construct the weapon with the specified parameters
	// AETVWeaponShieldBattery(FName NewName, float MaximumHP, float Dmg, float WeaponWeight, float WeaponSize, int32 Range);

	// Construct a random weapon depending on the Power LVL
	// AETVWeaponShieldBattery(FName NewName, int32 PowerLvl);
	
	
};
