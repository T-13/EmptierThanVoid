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
	
	// TODO When AETVShip is implemented
	//TSubclassOf<AETVShip> SelectedTarget;
	
public:
	// Sets default values for this actor's properties
	UETVActionTarget();

protected:
	// Required type of target
	// TODO When AETVShip is implemented
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ETV Action|Target")
	//TSubclassOf<AETVShip> RequiredTargetType;

	// Chance of failure when action is used
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ETV Action|Target", meta = (ClampMin = "0.0", ClampMax = "100.0"))
	float FailureChance;

public:
	UFUNCTION()
	virtual void ApplyEffectsTarget() {}

	// TODO When AETVShip is implemented
	//virtual bool IsTargetValid(TSubclassOf<AETVShip> Target);
};
