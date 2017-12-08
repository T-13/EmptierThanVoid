// Copyright (C) Team13. All rights reserved.

#include "ETVActionTarget.h"
#include "ETVGameModeBase.h" // Not in .h due to circular dependency

// Sets default values
UETVActionTarget::UETVActionTarget() : Super()
{
	// No failure possibility
	FailureChance = 0.0f;
}

void UETVActionTarget::SetTarget(TSubclassOf<UObject*> Target)
{
	SelectedTarget = Target;
}

bool UETVActionTarget::IsTargetValid(TSubclassOf<UObject*> Target)
{
	// Compare actors directly?
	//return (*Target)->GetClass()->IsChildOf((*RequiredTargetType)->GetClass());
	return true;
}

bool UETVActionTarget::CanPerform()
{
	return Super::CanPerform() && IsTargetValid(SelectedTarget);
}

bool UETVActionTarget::Activate()
{
	if (Super::Activate())
	{
		// Start targeting system
		AETVGameModeBase* GameMode = (AETVGameModeBase*)GetWorld()->GetAuthGameMode();
		GameMode->StartTargeting(this);

		return true;
	}

	return false;
}

void UETVActionTarget::Perform()
{
	Super::Perform();

	ApplyEffectsTarget();
}
