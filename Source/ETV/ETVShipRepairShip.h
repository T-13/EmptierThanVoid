// Copyright (C) Team13. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "ETVShip.h"
#include "ETVShipRepairShip.generated.h"

/**
 * 
 */
UCLASS()
class ETV_API AETVShipRepairShip : public AETVShip
{
	GENERATED_BODY()
	

public:
	// Sets default values for this actor's properties
	// TODO:: AETVShipRepairShip();

protected:
	// Maximum how much health can be regained when repairing some other Ship
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ETV Ship|RepairShip")
	int32 RepairPower;

public:
	// TODO Add methods
	// RepairShip(AETVShip ship);



	
	
};
