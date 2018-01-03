// Copyright (C) Team13. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "ETVActionTarget.h"
#include "ETVActionTarget_Use.generated.h"

/**
 * Action class for using a weapon on another asset.
 */
UCLASS()
class ETV_API UETVActionTarget_Use : public UETVActionTarget
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	UETVActionTarget_Use();

public:
	bool CanActivate() override;

	void ApplyEffectsTarget() override;
	void ApplyEffectsSelf() override;
};
