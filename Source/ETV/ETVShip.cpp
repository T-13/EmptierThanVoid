// Copyright (C) Team13. All rights reserved.

#include "ETVShip.h"
#include "UserWidget.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "ETVGameModeBase.h"

// Sets default values
AETVShip::AETVShip() : Super()
{
	// Set this actor to call Tick() every frame
	PrimaryActorTick.bCanEverTick = true;

	IsContextMenuOpen = false;

	Type = EETVShipType::PlayerShip;
}

void AETVShip::Init(FString NewName, int32 HP, int32 MaxHP, int32 ShieldP, int32 NewShieldRechargeTime, int32 NewSize, int32 NewMoveRange, int32 Speed)
{
	Name = NewName;
	HealthPoints = HP;
	MaximumHealthPoints = MaxHP;
	ShieldPoints = ShieldP;
	ShieldRechargeTime = NewShieldRechargeTime;
	Size = NewSize;
	MoveRange = NewMoveRange;
	ShipSpeed = Speed;
}

void AETVShip::BeginPlay()
{
	Super::BeginPlay();
	// Assigns click event to Spawn the Ship's ContextMenu
	OnClicked.AddDynamic(this, &AETVShip::SpawnContextMenu);
}

// Called every frame
void AETVShip::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Invoke custom Tick (from this/owned Actor) due to UE-22374
	if (IsContextMenuOpen && CurrentContextMenu != nullptr)
	{
		CurrentContextMenu->Ticked();
	}
}

void AETVShip::SetCurrentPosition(int32 NewX, int32 NewY)
{
	// Update references
	X = NewX;
	Y = NewY;
}

void AETVShip::AddWeapon(UETVWeaponSlot * Weapon)
{
	Weapons.Add(Weapon);
}

void AETVShip::AddAction(UETVAction* Action)
{
	Actions.Add(Action);
}

TArray<UETVWeaponSlot*> AETVShip::GetWeapons()
{
	return Weapons;
}

void AETVShip::RechargeShields()
{
}

void AETVShip::GetReport()
{
}

void AETVShip::SpawnContextMenu(AActor *Actor, FKey Key)
{
	AETVGameModeBase* GameMode = Cast<AETVGameModeBase>(GetWorld()->GetAuthGameMode());
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (ContextMenuClass != nullptr && !GameMode->IsTargeting() && !IsContextMenuOpen && !PlayerController->IsPaused())
	{
		// If another menu is in focus close it
		if (GameMode->WasShipClickedRecently())
		{
			GameMode->GetLastClickedShip()->UnconditionallyCloseContextMenu();
		}
		GameMode->ShipClicked(this);

		CurrentContextMenu = CreateWidget<UETVShipContextMenuWidget>(GetWorld(), ContextMenuClass);
		CurrentContextMenu->AssignShip(this);
		if (CurrentContextMenu != nullptr)
		{
			const FVector WorldTilePos = GetActorLocation();
			FVector2D ScreenTilePos;
			if (PlayerController->ProjectWorldLocationToScreen(WorldTilePos, ScreenTilePos))
			{
				CurrentContextMenu->SetPositionInViewport(ScreenTilePos);
				CurrentContextMenu->SetDesiredSizeInViewport(FVector2D(320, 280));
				CurrentContextMenu->AddToViewport();
				IsContextMenuOpen = true;
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("AETVShip::SpawnContextMenu(): Out of screen location!"))
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
	if (newValue >= 0 || newValue <= MaximumHealthPoints)
		HealthPoints = newValue;
	else if (newValue < 0)
		HealthPoints = 0;
	else if (newValue > MaximumHealthPoints)
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

bool AETVShip::CanMove()
{
	return ShipSpeed != 0;
}

void AETVShip::MoveToTile(int32 NewX, int32 NewY)
{
	AETVGameModeBase* GameMode = Cast<AETVGameModeBase>(GetWorld()->GetAuthGameMode());

	// Move tile
	GameMode->SetPosition(NewX, NewY, X, Y);

	// Move actor
	SetActorLocation(GameMode->GetPosition(NewX, NewY));

	// Update references
	SetCurrentPosition(NewX, NewY);
}

bool AETVShip::IsEnemy()
{
	return Type == EETVShipType::EnemyShip;
}

void AETVShip::CloseContextMenu()
{
	if (CurrentContextMenu != nullptr)
	{
		CurrentContextMenu->Close();
	}
}

void AETVShip::UnconditionallyCloseContextMenu()
{
	if (CurrentContextMenu != nullptr)
	{
		CurrentContextMenu->ShouldNotReOpen();
		CurrentContextMenu->Close();
	}
}

float AETVShip::GetMultiplier()
{
	// Calculate how much HealthPoints the ship has compared to its' initial HealthPoints
	float HpStatus = HealthPoints / MaximumHealthPoints;
	// If between 75% and 100% retrun this percentege
	if (HpStatus >= 0.75 && HpStatus <= 1)
		return HpStatus;
	// Weaker ships loose less per HP lost - We don't want ships to become useless
	else if (HpStatus < 0.75 && HpStatus >= 0.50)
	{
		// 75 is the minimal value for the previous case
		float Difference = 0.75 - HpStatus;
		// From 0.75 to 0.50 percenteges decresse 2 times slower
		Difference = Difference / 2;
		return (0.75 - Difference);
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
	// If this is the current focused menu tell gameMode it has closed
	AETVGameModeBase* GameMode = Cast<AETVGameModeBase>(GetWorld()->GetAuthGameMode());
	if (GameMode->GetLastClickedShip() == this)
	{
		GameMode->ShipClicked(nullptr);
	}

	// Tell the ship its ContextMenu is closed 
	IsContextMenuOpen = false;
	// Remove the reference to the Closed Menu
	CurrentContextMenu = nullptr;
}

void AETVShip::SetContextMenu(TSubclassOf<class UETVShipContextMenuWidget> ContextMenu)
{
	ContextMenuClass = ContextMenu;
}

void AETVShip::SetTypeToEnemy()
{
	Type = EETVShipType::EnemyShip;
}

FString AETVShip::GetShipType()
{
	if (Type == EETVShipType::PlayerShip)
		return "Player";
	else if (Type == EETVShipType::EnemyShip)
		return "Enemy";
	return "";
}

FString AETVShip::GetShipName()
{
	return Name;
}
