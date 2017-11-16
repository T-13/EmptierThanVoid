// Copyright (C) Team13. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "PaperSpriteActor.h"
#include "ETVWeapon.generated.h"

/**
* Abstract base Weapon class.
* Weapons are attached to ships.
*/
UCLASS(Abstract)
class ETV_API AETVWeapon : public APaperSpriteActor
{
    GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AETVWeapon();

protected:
	//Display weapon's name
    UPROPERTY(EditAnywhere)
    FName Name;

	// Weapons HitPoints
    UPROPERTY(EditAnywhere, meta = (ClampMin = "0.0", ClampMax = "100.0"))
    float HP;

	// DMG this weapon deals
    UPROPERTY(EditAnywhere)
    float Damage;

	// Type of this weapon
	// For easier identification for players
    UPROPERTY(EditAnywhere)
    FString Type;

	// The weight of the weapon
    UPROPERTY(EditAnywhere)
    float Weight;

	// Protection of the weapon
    UPROPERTY(EditAnywhere)
    float Armour;

	// The angles between which this weapon can hit
    UPROPERTY(EditAnywhere)
    int32 Angle[2];

	// Action point required to complete an action with this weapon
	// Might get removed
    UPROPERTY(EditAnywhere)
    int32 ActionPointsPerAction;

	// The range of the weapon
    UPROPERTY(EditAnywhere)
    int32 TravelDistance;

public:

    // Method to tell the weapon to fire.
    UFUNCTION(BlueprintCallable)
    virtual void Fire(int32 x, int32 y);

    // Get targets that weapon can hit
    UFUNCTION(BlueprintCallable)
    virtual TArray<int32> GetPossibleTargets();   // TODO-Object AETVShip not yet defined 

    //Gives the requirments for the weapon slot
    UFUNCTION(BlueprintCallable)
    virtual int32 GetSlotSpecifications();    // TODO-Object SlotSpecification not yet defined 

    // Get current status
    UFUNCTION(BlueprintCallable)
    virtual int32 GetReport(); // TODO-Object REPORT not yet defined 
};
