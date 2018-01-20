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
	AETVShipCapital();
	void Init(FString NewName, int32 MaxHP, int32 ShieldP, int32 NewShieldRechargeTime, int32 NewMoveRange, int32 Speed, bool bHyperDrive, float NewLaserResistance, int32 Hangars);

	void InitRandom(FString NewName);

	void InitRandomWithLevel(FString NewName, int32 PowerLvl);


protected:
	// If ship has a hyperdrive
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ETV Ship|Capital")
	bool bHasHyperdrive;

	// Affects damage done by laser
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ETV Ship|Capital", meta = (ClampMin = "0.0"))
	float LaserResistance;

	// Number of hangars smaller ships can go into and repair
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ETV Ship|Capital", meta = (ClampMin = "0"))
	int32 NumOfHangars;

public:
	UFUNCTION()
	void SpawnWeapons();

	UFUNCTION(Category = "AI")
	int32 GetScore() override;

	// TODO Add methods
	// UseHyperdrive(int32 x, int32 y);
	// RepairShipInHangar(AETVShip ship, int32 hangar);
};
