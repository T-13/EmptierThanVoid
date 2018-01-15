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
	// Construct the weapon with the specified parameters
	void Init(FName NewName, float MaximumHP, float Dmg, float WeaponWeight, float WeaponSize, int32 Range);

	// Construct a random weapon depending on the Power LVL
	virtual void InitRandom(FName NewName, int32 PowerLvl);

protected:
	// Display weapon's name
    UPROPERTY(EditAnywhere)
    FName Name;

	// Health Points of the Weapon
    UPROPERTY(EditAnywhere, meta = (ClampMin = "0.0"))
    float HealthPoints;

	// Maximum Health Points of the Weapon
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0.0"))
	float MaximumHealthPoints;

	// DMG this weapon deals
    UPROPERTY(EditAnywhere)
    float Damage;

	// Type of this weapon
	// For easier identification for players
    UPROPERTY()
    int32 Type;

	// The weight of the weapon
    UPROPERTY(EditAnywhere)
    float Weight;

	// Size of the weapon
    UPROPERTY(EditAnywhere)
    float Size;

	// TODO The angles between which this weapon can hit
    // UPROPERTY(EditAnywhere)
	// TArray<int32> Angle;

	// The range of the weapon
    UPROPERTY(EditAnywhere)
    int32 FiringRange;

public:
	UFUNCTION()
	FString GetDisplayString() const { return Name.ToString(); }

	// TODO Get the angle at which the weapon can fire
	// UFUNCTION(BlueprintCallable)
	// TArray<int32> GetAngle();

	// Gives the Weight requirment for the weapon slot
	UFUNCTION(BlueprintCallable)
	float GetWeight();

	// Gives the Size requirments for the weapon slot
	UFUNCTION(BlueprintCallable)
	float GetSize();

	// Get current status
	UFUNCTION(BlueprintCallable)
	virtual int32 GetReport(); // TODO-Object REPORT not yet defined

	// Get current status
	UFUNCTION(BlueprintCallable)
	int32 GetType();

	// Get weapon damage
	UFUNCTION(BlueprintCallable)
	float GetDMG();

	// Set weapons current HealthPoints
	void SetHP(int32 newValue);

	// Get weapons current HealthPoints
	int32 GetHP();

	// Get weapons current firing range
	int32 GetFiringRange() const { return FiringRange; }

	// Static intigers to define type of effect a weapon has
	const static int32 DamageHull = 1;
	const static int32 DamageShieldThenHull = 2;
	const static int32 HealHull = 3;
	const static int32 HealShield = 4;
};
