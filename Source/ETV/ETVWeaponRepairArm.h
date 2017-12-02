// Copyright (C) Team13. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "ETVWeapon.h"
#include "ETVWeaponRepairArm.generated.h"

/**
 * Heals HealthPoints of friendly Ships
 */
UCLASS()
class ETV_API AETVWeaponRepairArm : public AETVWeapon
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AETVWeaponRepairArm();

	// Construct the weapon with the specified parameters
	AETVWeaponRepairArm(FName NewName, float MaximumHP, float Dmg, float WeaponWeight, float WeaponSize, int32 Range);

	// Construct a random weapon depending on the Power LVL
	AETVWeaponRepairArm(FName NewName, int32 PowerLvl);
	
	
};
