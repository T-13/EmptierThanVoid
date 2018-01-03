// Copyright (C) Team13. All rights reserved.

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
	AETVShipFighter();
	void Init(FString NewName, int32 MaxHP, int32 ShieldP, int32 NewShieldRechargeTime, int32 NewMoveRange, int32 Speed, float Deflection);

	void InitRandom(FString NewName);

	void InitRandomWithLevel(FString NewName, int32 PowerLvl);


protected:
	// Chance for ship to deflect an attack
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ETV Ship|Fighter", meta = (ClampMin = "0.0", ClampMax = "100.0"))
	float DeflectionChance;
	
public:
	// TODO Add methods
	
};
