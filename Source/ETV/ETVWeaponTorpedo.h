// Copyright (C) Team13. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "ETVWeapon.h"
#include "ETVWeaponTorpedo.generated.h"

/**
 * A missile based weapon that ignores shields
 */
UCLASS()
class ETV_API AETVWeaponTorpedo : public AETVWeapon
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AETVWeaponTorpedo();
	// Construct the weapon with the specified parameters
	void Init(FName NewName, float MaximumHP, float Dmg, float WeaponWeight, float WeaponSize, int32 Range, int32 Speed);

	// Construct a random weapon depending on the Power LVL
	void InitRandom(FName NewName, int32 PowerLvl) override;

protected:
	// The speed of the sheep
	int32 SquaresPerTurn;

public:
	// Returns the speed of the ship
	int32 GetSquaresPerTurn();

};
