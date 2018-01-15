// Copyright (C) Team13. All rights reserved.

#include "ETVActionTarget_Move.h"
#include "ETVShip.h"
#include "PaperTileMapActor.h"
#include "ETVGameModeBase.h"

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
	if (Super::CanPerform() && TileX != -1 && TileY != -1)
	{
		AETVGameModeBase* GameMode = Cast<AETVGameModeBase>(GetWorld()->GetAuthGameMode());
		float Distance = GameMode->GetTiledDistance(OwnerShip->GetX(), OwnerShip->GetY(), TileX, TileY);

		return Distance <= OwnerShip->GetMoveRange();
	}

	return false;
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
