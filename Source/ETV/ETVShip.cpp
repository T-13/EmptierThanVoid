// Copyright (C) Team13. All rights reserved.

#include "ETVShip.h"
#include "UserWidget.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AETVShip::AETVShip() : Super()
{
	IsContextMenuOpen = false;
}

void AETVShip::BeginPlay()
{
	Super::BeginPlay();
	// Assigns click event to Spawn the Ship's ContextMenu
	OnClicked.AddDynamic(this, &AETVShip::SpawnContextMenu);
}

void AETVShip::GetCurrentPosition()
{
}

void AETVShip::RechargeShields()
{
}

void AETVShip::GetReport()
{
}

void AETVShip::SpawnContextMenu(AActor *Actor, FKey Key)
{
	if (ContextMenuClass != nullptr)
	{
		if(!IsContextMenuOpen){		
			CurrentContextMenu = CreateWidget<UETVShipContextMenuWidget>(GetWorld(), ContextMenuClass);
			CurrentContextMenu->AssignShip(this);
			if(CurrentContextMenu != nullptr)
			{
				float x; // X coordinate of MouseCursor
				float y; // Y coordinate of MouseCursor
				if(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetMousePosition(x,y)){
					CurrentContextMenu->SetPositionInViewport(UKismetMathLibrary::MakeVector2D(x,y));
					CurrentContextMenu->SetDesiredSizeInViewport(UKismetMathLibrary::MakeVector2D(320, 280));
					CurrentContextMenu->AddToViewport();
					IsContextMenuOpen = true;
				}
			}
		}
	}
}

int32 AETVShip::GetHP()
{
	return HealthPoints;
}

void AETVShip::SetHP(int32 newValue)
{
	if(newValue >= 0 || newValue <= MaximumHealthPoints)
		HealthPoints = newValue;
	else if(newValue < 0)
		HealthPoints = 0;
	else if(newValue > MaximumHealthPoints)
		HealthPoints = MaximumHealthPoints;
}

int32 AETVShip::GetShields()
{
	return ShieldPoints;
}

void AETVShip::SetShields(int32 newValue)
{
	if (newValue < 0)
		ShieldPoints = 0;
	else
		ShieldPoints = newValue;
}

float AETVShip::GetMultiplier()
{
	// Calculate how much HealthPoints the ship has compared to its' initial HealthPoints
	float HpStatus = HealthPoints/MaximumHealthPoints;
	// If between 75% and 100% retrun this percentege
	if (HpStatus >= 0.75 && HpStatus <= 1)
		return HpStatus;
	// Weaker ships loose less per HP lost - We don't want ships to become useless
	else if(HpStatus < 0.75 && HpStatus >= 0.50)
	{
		// 75 is the minimal value for the previous case
		float Difference = 0.75 - HpStatus;
		// From 0.75 to 0.50 percenteges decresse 2 times slower
		Difference = Difference / 2;
		return (0.75-Difference);
	}
	else if (HpStatus < 0.50 && HpStatus > 0.0)
	{
		// 0.50 is the minimal value for the previous case
		float Difference = 0.50 - HpStatus;
		// From 0.75 to 0.50 percenteges decresse 4 times slower
		Difference = Difference / 4;
		// 0.625 is the minimum Mupltiplier returned by previous case
		return (0.625 - Difference);
	}
	else
	{
		// If Ship is dead or MaxHP is less then Current HP
		// We shouldn't get here!
		UE_LOG(LogTemp, Error, TEXT("GetMultiplier(): Ship is dead or MaxHP is less then Current HP!"));
		return 0;	
	}
}

void AETVShip::ClosingContextMenu()
{
	// Tell the ship its' ContextMenu is closed 
	IsContextMenuOpen = false;
	// Remove the reference to the Closed Menu
	CurrentContextMenu = nullptr;
}

