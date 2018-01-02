// Copyright (C) Team13. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "ETVAction.h"
#include "ETVActionTarget.generated.h"

/**
 * Action class for actions performed on other assets.
 */
UCLASS(Blueprintable)
class ETV_API UETVActionTarget : public UETVAction
{
	GENERATED_BODY()


public:
	// Sets default values for this actor's properties
	UETVActionTarget();

protected:
	// Currently selected target (nullptr if none)
	UPROPERTY(BlueprintReadOnly, Category = "ETV Action|Target")
	AActor* SelectedTarget;

	// Currently selected tile map X coordinate (-1 if unset)
	UPROPERTY(BlueprintReadOnly, Category = "ETV Action|Target")
	int32 TileX;

	// Currently selected tile map Y coordinate (-1 if unset)
	UPROPERTY(BlueprintReadOnly, Category = "ETV Action|Target")
	int32 TileY;

	// Required type of target
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ETV Action|Target")
	TSubclassOf<AActor> RequiredTargetType;

	// Chance of failure when action is used
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ETV Action|Target", meta = (ClampMin = "0.0", ClampMax = "100.0"))
	float FailureChance;

public:
	void Init(AETVShip* OwnerShipPtr, AETVWeapon* OwnerWeaponPtr = nullptr) override;

	UFUNCTION()
	virtual void ApplyEffectsTarget() {}

	virtual void SetTarget(AActor* Target, int32 X = -1, int32 Y = -1);
	virtual bool IsTargetValid();

	bool CanActivate() override;
	bool CanPerform() override;
	bool Activate() override;
	bool Perform() override;
	void OnEndPerform() override;
};
