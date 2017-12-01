// Copyright (C) Team13. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ETVWeapon.h"
#include "ETVWeaponSlot.generated.h"

/**
 * 
 */
UCLASS()
class ETV_API UETVWeaponSlot : public UObject
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	UETVWeaponSlot();
	// Constructs the WeaponSlot with specified parameters
	UETVWeaponSlot(float Size, float Wieght);

protected:
	// The weapon holstered in this slot
	UPROPERTY(EditAnywhere)
	AETVWeapon* Weapon;

	// The maximum Weight of the weapon that can be put in this slot
	UPROPERTY(EditAnywhere)
	float MaxCarryWeight;

	// The maximum size of the weapon that can be put in this slot
	UPROPERTY(EditAnywhere)
	float MaxCarrySize;

public:
	// Returns true if the passed weapon can be put into this slot and false otherwise
	bool DoesWeaponFit(AETVWeapon* Weapon);

	// Tries to put the passed weapon into the slot. Returns true if succssesful and false otherwise
	bool FitWeapon(AETVWeapon* Weapon);

	// Tried to remove current weapon from the slot. Returns true if weapon was removed and false otherwise
	bool RemoveWeapon();

	// Returns the reference to the slotted weapon
	AETVWeapon* getWeapon();

	// Returns true if weapon is slotted and false otherwise
	bool hasWeapon();
	
};
