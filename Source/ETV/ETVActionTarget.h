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
	TSubclassOf<UObject> SelectedTarget;

	// Required type of target
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ETV Action|Target")
	TSubclassOf<UObject> RequiredTargetType;

	// Chance of failure when action is used
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ETV Action|Target", meta = (ClampMin = "0.0", ClampMax = "100.0"))
	float FailureChance;

public:
	UFUNCTION()
	virtual void ApplyEffectsTarget() {}

	virtual bool IsTargetValid(TSubclassOf<UObject> Target) PURE_VIRTUAL(UETVActionTarget::IsTargetValid, return false; );

	void Perform() override;
};
