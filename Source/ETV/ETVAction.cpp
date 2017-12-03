// Copyright (C) Team13. All rights reserved.

#include "ETVAction.h"

// Sets default values
UETVAction::UETVAction()
{
	Name = FName(TEXT("UnknownAction"));

	// Fits 0 to 100 size assets
	MinAttachSize = 0;
	MaxAttachSize = 100;

	// Range of 0 to 100 tiles
	MinRange = 0;
	MaxRange = 100;

	// Available
	Available = EETVActionAvailability::ActionAvailable;

	// Does not end turn
	bEndsTurn = false;
}

bool UETVAction::CanPerform()
{
	return Available == EETVActionAvailability::ActionAvailable;
}

void UETVAction::Perform()
{
	ApplyEffectsSelf();

	if (bEndsTurn)
	{
		// TODO Delay this if necessary
		AETVGameModeBase* GameMode = (AETVGameModeBase*)GetWorld()->GetAuthGameMode();
		GameMode->EndTurn();
	}
}

EETVActionAvailability UETVAction::ToggleAvailability()
{
	if (Available == EETVActionAvailability::ActionAvailable)
	{
		Available = EETVActionAvailability::ActionUnavailable;
	}
	else
	{
		Available = EETVActionAvailability::ActionAvailable;
	}

	return Available;
}
