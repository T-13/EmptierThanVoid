// Copyright (C) Team13. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "ETVActionTarget.h"
#include "ETVActionTarget_Fire.generated.h"

/**
 * Action class for firing at another asset.
 */
UCLASS()
class ETV_API UETVActionTarget_Fire : public UETVActionTarget
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	UETVActionTarget_Fire();

public:
	bool CanPerform() override;

	void ApplyEffectsTarget() override;
	void ApplyEffectsSelf() override;
};
