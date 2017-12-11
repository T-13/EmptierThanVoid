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

bool UETVActionTarget_Move::CanPerform()
{
	return Super::CanPerform() && OwnerShip->CanMove();
}

void UETVActionTarget_Move::ApplyEffectsSelf()
{
	if (IsLastPerform())
	{
		Super::ApplyEffectsSelf();
		// TODO Move from current tile to target tile, do flipbook animations and all that
	}
}
