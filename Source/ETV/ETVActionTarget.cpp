// Copyright (C) Team13. All rights reserved.

#include "ETVActionTarget.h"
#include "ETVGameModeBase.h" // Not in .h due to circular dependency

// Sets default values
UETVActionTarget::UETVActionTarget() : Super()
{
	// No failure possibility
	FailureChance = 0.0f;

	SelectedTarget = nullptr;
	TileX = -1;
	TileY = -1;
}

void UETVActionTarget::Init(AETVShip* OwnerShipPtr, AETVWeapon* OwnerWeaponPtr)
{
	Super::Init(OwnerShipPtr, OwnerWeaponPtr);

	if (OwnerWeapon != nullptr)
	{
		FString NameWithWeapon = Name.ToString() + " " + OwnerWeapon->GetDisplayString();
		Name = FName(*NameWithWeapon);
	}
}

void UETVActionTarget::SetTarget(AActor* Target, int32 X, int32 Y)
{
	SelectedTarget = Target;
	TileX = X;
	TileY = Y;
}

bool UETVActionTarget::IsTargetValid()
{
	// Are required variables set
	if (SelectedTarget == nullptr || RequiredTargetType == nullptr)
	{
		return false;
	}

	// Is correct type
	return SelectedTarget->IsA(RequiredTargetType);
}

bool UETVActionTarget::CanActivate()
{
	return Super::CanActivate();
}

bool UETVActionTarget::CanPerform()
{
	return Super::CanPerform() && IsTargetValid();
}

bool UETVActionTarget::Activate()
{
	if (Super::Activate())
	{
		// Start targeting system
		AETVGameModeBase* GameMode = Cast<AETVGameModeBase>(GetWorld()->GetAuthGameMode());
		GameMode->StartTargeting(this);

		return true;
	}

	return false;
}

bool UETVActionTarget::Perform()
{
	if (Super::Perform())
	{
		ApplyEffectsTarget();

		return true;
	}

	return false;
}
