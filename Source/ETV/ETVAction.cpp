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

	// Takes at most one turn to complete
	MaxPerforms = 1;
	CurrentPerform = 0;
}

UWorld* UETVAction::GetWorld() const
{
	if (OwnerShip != nullptr)
	{
		return OwnerShip->GetWorld();
	}
	return nullptr;
}

void UETVAction::Init(AETVShip* OwnerShipPtr, AETVWeapon* OwnerWeaponPtr)
{
	OwnerShip = OwnerShipPtr;
	OwnerWeapon = OwnerWeaponPtr;
}

bool UETVAction::IsMultiTurn()
{
	return MaxPerforms > 1;
}

bool UETVAction::IsFirstPerform()
{
	return CurrentPerform == 1;
}

bool UETVAction::IsLastPerform()
{
	return CurrentPerform >= MaxPerforms;
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
	OnBeginPerform();

	AETVGameModeBase* GameMode = Cast<AETVGameModeBase>(GetWorld()->GetAuthGameMode());

	if (IsFirstPerform() && !IsMultiTurn())
	{
		GameMode->AddMultiTurnAction(this);
	}

	if (IsLastPerform() && !IsMultiTurn())
	{
		GameMode->RemoveMultiTurnAction(this);

		ApplyEffectsSelf();
	}

	OnEndPerform();
}

void UETVAction::OnBeginPerform()
{
	++CurrentPerform;
}

void UETVAction::OnEndPerform()
{
	AETVGameModeBase* GameMode = (AETVGameModeBase*)GetWorld()->GetAuthGameMode();
	GameMode->GetShipListWidget()->Update();

	if (bEndsTurn)
	{
		AETVGameModeBase* GameMode = Cast<AETVGameModeBase>(GetWorld()->GetAuthGameMode());
		// TODO Delay this until all effects are done
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
