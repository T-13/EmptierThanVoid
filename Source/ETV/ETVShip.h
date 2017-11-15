// Copyright (C) Team13. All rights reserved.

#pragma once

#include "ETVAction.h"
#include "CoreMinimal.h"
#include "PaperSpriteActor.h"
#include "ETVShip.generated.h"

/**
 * Abstract base Ship class.
 */
UCLASS(NotPlaceable, Abstract)
class ETV_API AETVShip : public APaperSpriteActor
{
	GENERATED_BODY()
	

public:
	// Sets default values for this actor's properties
	// TODO AETVShip();

protected:
	// Name of the ship
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ETV Ship")
	FName Name;

	// Health points of the ship
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ETV Ship", meta = (ClampMin = "0.0"))
	int32 HealthPoints;
	
	// Bonus to health for ships
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ETV Ship", meta = (ClampMin = "0.0"))
	int32 ShieldPoints;

	// Time for the shields to recharge
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ETV Ship", meta = (ClampMin = "0.0"))
	int32 ShieldRechargeTime;

	// Weapon slots
	// TODO When AETVWeapon is implemented

	// Integers (x,y) that represent a point on a sprite where the attachement goes
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ETV Ship")
	TArray<int32> AttachementSlots;

	// Size of the ship
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ETV Ship", meta = (ClampMin = "1.0"))
	int32 Size;

	// Area that ship is allowed to move to
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ETV Ship", meta = (ClampMin = "1.0"))
	int32 MoveRange;

	// Integers (x,y) for possible fields that ship can move to
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ETV Ship")
	TArray<int32> Fields;

	// Action avaible on ship
	TArray<UETVAction> Actions;

	// How fast ship moves
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ETV Ship", meta = (ClampMin = "1.0"))
	int32 ShipSpeed;


public:
	UFUNCTION()
	virtual void GetCurrentPosition();

	UFUNCTION()
	virtual void RechargeShields();

	UFUNCTION()
	virtual void GetReport();

};
