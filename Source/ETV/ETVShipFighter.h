﻿// Copyright (C) Team13. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "ETVShip.h"
#include "ETVShipFighter.generated.h"

/**
 * Class for fighter ships.
 */
UCLASS()
class ETV_API AETVShipFighter : public AETVShip
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	//TODO AETVShipFighter();

protected:
	// Chance for ship to deflect an attack
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ETV Ship|Fighter", meta = (ClampMin = "0.0", ClampMax = "100.0"))
	float DeflectionChance;

	// Affects the movement speed
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ETV Action|Target", meta = (ClampMin = "0.0"))
	float Acceleration;
	
public:
	//TODO Add methods
	
};