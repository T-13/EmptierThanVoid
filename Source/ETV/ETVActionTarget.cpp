// Copyright (C) Team13. All rights reserved.

#include "ETVActionTarget.h"

// Sets default values
UETVActionTarget::UETVActionTarget() : Super()
{
	// No failure possibility
	FailureChance = 0.0f;
}

void UETVActionTarget::SetTarget(TSubclassOf<UObject> Target)
{
	SelectedTarget = Target;
}

bool UETVActionTarget::IsTargetValid(TSubclassOf<UObject> Target)
{
	// Compare actors directly?
	//return (*Target)->GetClass()->IsChildOf((*RequiredTargetType)->GetClass());
	return true;
}

bool UETVActionTarget::CanPerform()
{
	return Super::CanPerform() && IsTargetValid(SelectedTarget);
}

void UETVActionTarget::Perform()
{
	Super::Perform();

	ApplyEffectsTarget();
}
