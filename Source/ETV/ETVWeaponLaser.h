// Copyright (C) Team13. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "ETVWeapon.h"
#include "ETVWeaponLaser.generated.h"

/**
 * A powerful weapon that fires immediately but is blocked by shields
 */
UCLASS()
class ETV_API AETVWeaponLaser : public AETVWeapon
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AETVWeaponLaser();
	
	// Construct the weapon with the specified parameters
	// AETVWeaponLaser(FName NewName, float MaximumHP, float Dmg, float WeaponWeight, float WeaponSize, int32 Range);

	// Construct a random weapon depending on the Power LVL
	// AETVWeaponLaser(FName NewName, int32 PowerLvl);
};
