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

void AETVShip::ClosingContextMenu()
{
	// Tell the ship its' ContextMenu is closed 
	IsContextMenuOpen = false;
	// Remove the reference to the Closed Menu
	CurrentContextMenu = nullptr;
}
