// Copyright (C) Team13. All rights reserved.

#include "ETVActionTarget.h"
#include "ETVGameModeBase.h" // Not in .h due to circular dependency

// Sets default values
UETVActionTarget::UETVActionTarget() : Super()
{
	// No failure possibility
	FailureChance = 0.0f;

	SelectedTarget = nullptr;
	Tile = FVector2D(-1, -1);
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
	Tile = FVector2D(X, Y);
}

bool UETVActionTarget::IsTargetValid()
{
	// Are required variables set and is correct type
	return SelectedTarget != nullptr && RequiredTargetType != nullptr && SelectedTarget->IsA(RequiredTargetType);
}

bool UETVActionTarget::CanActivate()
{
	return Super::CanActivate();
}

bool UETVActionTarget::CanPerform()
{
	if (Super::CanPerform() && IsTargetValid())
	{
		if (OwnerWeapon != nullptr && Tile.X != -1 && Tile.Y != -1) {
			AETVGameModeBase* GameMode = Cast<AETVGameModeBase>(GetWorld()->GetAuthGameMode());
			float Distance = GameMode->GetTiledDistance(OwnerShip->GetTilePosition(), Tile);

			return Distance <= OwnerWeapon->GetFiringRange();
		}

		return true;
	}

	return false;
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

		OnEndPerform();

		return true;
	}

	return false;
}

void UETVActionTarget::OnEndPerform()
{
	AETVGameModeBase* GameMode = Cast<AETVGameModeBase>(GetWorld()->GetAuthGameMode());
	GameMode->GetShipListWidget()->Update();
}
