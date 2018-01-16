// Copyright (C) Team13. All rights reserved.

#include "ETVActionTarget_Fire.h"
#include "ETVGameModeBase.h"
#include "ETVCalculator.h"
#include "PaperTileMapActor.h"

// Sets default values
UETVActionTarget_Fire::UETVActionTarget_Fire() : Super()
{
	Name = FName(TEXT("Fire"));

	// Set required target type to general ship
	RequiredTargetType = AETVShip::StaticClass();
}

bool UETVActionTarget_Fire::IsTargetValid()
{
	if (Super::IsTargetValid())
	{
		return true;
	}

	// Are required variables set (repeated check from parent due to special condition)
	if (SelectedTarget != nullptr && RequiredTargetType != nullptr)
	{
		// Is target a valid tile
		if (SelectedTarget->IsA(APaperTileMapActor::StaticClass()) && Tile.X != -1 && Tile.Y != -1)
		{
			// Is tile not visible (fog is accessible no matter the content)
			AETVGameModeBase* GameMode = Cast<AETVGameModeBase>(GetWorld()->GetAuthGameMode());
			if (!GameMode->IsTileVisible(Tile, OwnerShip->GetType()))
			{
				return true;
			}
		}
	}

	return false;
}

bool UETVActionTarget_Fire::CanActivate()
{
	// Check if a weapon is set
	return Super::CanActivate() && OwnerWeapon != nullptr;
}

bool UETVActionTarget_Fire::CanPerform()
{
	if (Super::CanPerform())
	{
		// Check if fog
		if (SelectedTarget->IsA(APaperTileMapActor::StaticClass()))
		{
			return true;
		}

		// Check if enemy
		AETVShip* SelectedShip = Cast<AETVShip>(SelectedTarget); // Required type is ship (checked in parent) so casting is safe
		return SelectedShip->IsEnemy();
	}

	return false;
}

void UETVActionTarget_Fire::ApplyEffectsTarget()
{
	Super::ApplyEffectsTarget();

	// TODO Show explosion animation

	AETVShip* SelectedShip = nullptr;
	bool bLog = true;

	if (!SelectedTarget->IsA(APaperTileMapActor::StaticClass()))
	{
		AETVGameModeBase* GameMode = Cast<AETVGameModeBase>(GetWorld()->GetAuthGameMode());
		SelectedShip = GameMode->GetShipActor(Tile.X, Tile.Y);
		bLog = false;
	}
	else
	{
		SelectedShip = Cast<AETVShip>(SelectedTarget); // Required type is ship (checked in parent) so casting is safe
	}

	if (SelectedShip != nullptr)
	{
		UETVCalculator::CalculateWeaponEffect(OwnerShip, OwnerWeapon, SelectedShip, bLog);
	}
}

void UETVActionTarget_Fire::ApplyEffectsSelf()
{
	Super::ApplyEffectsSelf();

	// TODO Lower ammo count, recharge or similar
}
