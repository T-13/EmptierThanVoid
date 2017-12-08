// Copyright (C) Team13. All rights reserved.

#include "ETVActionTarget_Move.h"

// Sets default values
UETVActionTarget_Move::UETVActionTarget_Move() : Super()
{
	// Set required target type to tile
}

bool UETVActionTarget_Move::CanPerform()
{
	// TODO Check if tile is selected target, check if drives are operable
	return Super::CanPerform();
}

void UETVActionTarget_Move::ApplyEffectsSelf()
{
	Super::ApplyEffectsSelf();
	// TODO Move from current tile to target tile, do flipbook animations and all that
}
