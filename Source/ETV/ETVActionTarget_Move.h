// Copyright (C) Team13. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "ETVActionTarget.h"
#include "ETVActionTarget_Move.generated.h"

/**
 * Action class for movement to other tiles.
 */
UCLASS()
class ETV_API UETVActionTarget_Move : public UETVActionTarget
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	UETVActionTarget_Move();

public:
	bool CanActivate() override;
	bool CanPerform() override;

	void ApplyEffectsSelf() override;
};
