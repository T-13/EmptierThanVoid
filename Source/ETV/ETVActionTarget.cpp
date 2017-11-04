// Copyright (C) Team13. All rights reserved.

#include "ETVActionTarget.h"

// Sets default values
UETVActionTarget::UETVActionTarget() : Super()
{
	// No failure possibility
	FailureChance = 0.0f;
}

void UETVActionTarget::Perform()
{
	Super::Perform();

	ApplyEffectsTarget();
}
