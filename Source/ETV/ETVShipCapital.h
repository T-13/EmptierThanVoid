// Copyright (C) Team13. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "ETVShip.h"
#include "ETVShipCapital.generated.h"

/**
 * 
 */
UCLASS()
class ETV_API AETVShipCapital : public AETVShip
{
	GENERATED_BODY()


public:
	// Sets default values for this actor's properties
	//TODO AETVShipCapital();

protected:
	// If ship has a hyperdrive
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ETV Ship|Fighter")
	bool hasHyperdrive;

	// Affects damage done by laser
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ETV Action|Target", meta = (ClampMin = "0.0"))
	float laserREsistance;

	// Number of hangars smaller ships can go into and repair
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ETV Action|Target", meta = (ClampMin = "0"))
	int32 NumOfHangars;

public:
	//TODO Add methods
	//UseHyperdrive(int32 x, int32 y);
	//RepairShipInHangar(AETVShip ship, int32 hangar);
	
	
	
};
