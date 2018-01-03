// Copyright (C) Team13. All rights reserved.

#include "ETVActionTarget_Move.h"
#include "ETVShip.h"
#include "PaperTileMapActor.h"

// Sets default values
UETVActionTarget_Move::UETVActionTarget_Move() : Super()
{
	Name = FName(TEXT("Move"));

	// Set required target type to tile
	RequiredTargetType = APaperTileMapActor::StaticClass();
}

bool UETVActionTarget_Move::CanActivate()
{
	return Super::CanActivate() && OwnerShip->CanMove();
}

bool UETVActionTarget_Move::CanPerform()
{
	// TODO Check distance (Ship's MoveRange vs distance)
	return Super::CanPerform() && TileX != -1 && TileY != -1 && OwnerShip->CanMove();
}

void UETVActionTarget_Move::ApplyEffectsSelf()
{
	Super::ApplyEffectsSelf();

	if (IsLastPerform())
	{
		// TODO Do animation

		// Move ship
		OwnerShip->MoveToTile(TileX, TileY);
	}
}
