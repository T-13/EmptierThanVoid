// Copyright (C) Team13. All rights reserved.

#include "ETVAction.h"
#include "ETVGameModeBase.h" // Not in .h due to circular dependency

// Sets default values
UETVAction::UETVAction()
{
	OwnerShip = nullptr;
	OwnerWeapon = nullptr;

	Name = FName(TEXT("UnknownAction"));

	// Range of 0 to 100 tiles
	MinRange = 0;
	MaxRange = 100;

	// Available
	Available = EETVActionAvailability::ActionAvailable;

	// Does not end turn
	bEndsTurn = false;
}

void UETVAction::Init(AETVShip* OwnerShipPtr, AETVWeapon* OwnerWeaponPtr)
{
	OwnerShip = OwnerShipPtr;
	OwnerWeapon = OwnerWeaponPtr;
}


bool UETVAction::CanPerform()
{
	// TODO Check range
	return Available == EETVActionAvailability::ActionAvailable && OwnerShip != nullptr;
}

bool UETVAction::Activate()
{
	// Double check if we can perform, then perform
	if (CanPerform())
	{
		Perform();
		return true;
	}
	return false;
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
