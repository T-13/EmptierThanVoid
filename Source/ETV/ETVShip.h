﻿// Copyright (C) Team13. All rights reserved.

#pragma once

#include "ETVAction.h"
#include "ETVWeaponSlot.h"
#include "CoreMinimal.h"
#include "PaperSpriteActor.h"
#include "ETVShipContextMenuWidget.h"
#include "ETVShip.generated.h"

/**
 * Abstract base Ship class.
 */
UCLASS(Abstract)
class ETV_API AETVShip : public APaperSpriteActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AETVShip();

protected:
	// Called when the game starts or when spawned
	void BeginPlay() override;

protected:

	// Name of the ship
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ETV Ship")
	FName Name;

	// Health points of the ship
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ETV Ship", meta = (ClampMin = "0.0"))
	int32 HealthPoints;

	// Maximum health points of the ship
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ETV Ship", meta = (ClampMin = "0.0"))
	int32 MaximumHealthPoints;
	
	// Bonus to health for ships
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ETV Ship", meta = (ClampMin = "0.0"))
	int32 ShieldPoints;

	// Time for the shields to recharge
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ETV Ship", meta = (ClampMin = "0.0"))
	int32 ShieldRechargeTime;

	// Weapon slots
	TArray<UETVWeaponSlot> Weapons;

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

	// Actions avaible on ship
	TArray<UETVAction> Actions;

	// How fast ship moves
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ETV Ship", meta = (ClampMin = "1.0"))
	int32 ShipSpeed;

	// The widget class for the ContextMenu
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ETV Ship", meta = (BlueprintProtected = "true"))
	TSubclassOf<class UETVShipContextMenuWidget> ContextMenuClass;

	// Instance of our ContextMenu
	UPROPERTY()
	class UETVShipContextMenuWidget* CurrentContextMenu;

	// Tells us if ContextMenu for this ship is Open
	UPROPERTY()
	bool IsContextMenuOpen;

public:
	UFUNCTION()
	virtual void GetCurrentPosition();

	UFUNCTION()
	virtual void RechargeShields();

	UFUNCTION()
	virtual void GetReport();

	UFUNCTION()
	void SpawnContextMenu(AActor *Actor, FKey Key);

	UFUNCTION()
	int32 GetHP();

	UFUNCTION()
	void SetHP(int32 newValue);

	UFUNCTION()
	int32 GetShields();

	UFUNCTION()
	void SetShields(int32 newValue);


	// Returns a multiplier for the effectiveness of ship's actions depending on its status
	UFUNCTION()
	virtual float GetMultiplier();

	UFUNCTION(BlueprintCallable)
	void ClosingContextMenu();

	UFUNCTION(BlueprintCallable)
	void SetContextMenu(TSubclassOf<class UETVShipContextMenuWidget> ContextMenu);

};
