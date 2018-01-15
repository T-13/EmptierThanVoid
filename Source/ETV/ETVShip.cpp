// Copyright (C) Team13. All rights reserved.

#include "ETVShip.h"
#include "UserWidget.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "ETVGameModeBase.h"
#include "ETVWeaponLaser.h"

// Sets default values
AETVShip::AETVShip() : Super()
{
	// Set this actor to call Tick() every frame
	PrimaryActorTick.bCanEverTick = true;

	IsContextMenuOpen = false;

	Type = EETVShipType::PlayerShip;

	// Default ranges
	MoveRange = 1;
	SensorRange = 2;
}

void AETVShip::Init(FString NewName, int32 MaxHP, int32 ShieldP, int32 NewShieldRechargeTime, int32 NewMoveRange, int32 Speed)
{
	Name = NewName;
	HealthPoints = MaxHP;
	MaximumHealthPoints = MaxHP;
	ShieldPoints = ShieldP;
	ShieldRechargeTime = NewShieldRechargeTime;
	MoveRange = NewMoveRange;
	ShipSpeed = Speed;
}

void AETVShip::InitRandom(FString NewName)
{
	Level = FMath::RandRange(50, 200);
	InitRandomWithLevel(NewName, Level);
}

void AETVShip::InitRandomWithLevel(FString NewName, int32 PowerLvl)
{
	if (PowerLvl < 50)
		PowerLvl = 50;
	else if (PowerLvl > 200)
		PowerLvl = 200;

	Level = PowerLvl;

	// The range of values we can generate
	int32 RangeMin = PowerLvl - 25;
	int32 RangeMax = PowerLvl + 25;

	Name = NewName;

	MaximumHealthPoints = FMath::RandRange(RangeMin, RangeMax);
	HealthPoints = MaximumHealthPoints;

	ShieldPoints = FMath::RandRange(RangeMin, RangeMax);

	// The better the level the lower the time (min 1)
	int32 MinShieldRechargeTime = 200 / PowerLvl;
	int32 MaxShieldRechargeTime = MinShieldRechargeTime + 3;
	ShieldRechargeTime = FMath::RandRange(MinShieldRechargeTime, MaxShieldRechargeTime);

	int32 MinMoveRange = PowerLvl / 25;
	int32 MaxMoveRange = PowerLvl / 15;
	MoveRange = FMath::RandRange(MinMoveRange, MaxMoveRange);

	ShipSpeed = FMath::RandRange(MinMoveRange, MaxMoveRange);

	SpawnWeapons();
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
		if (WasRecentlyRendered())
		{
			CurrentContextMenu->Ticked();
		}
		else
		{
			// Close if off-screen (prevent close animation from restarting/pausing mid-way)
			ClosingContextMenu();
		}
	}
}

void AETVShip::SetCurrentPosition(int32 NewX, int32 NewY)
{
	// Update references
	X = NewX;
	Y = NewY;

	AETVGameModeBase* GameMode = Cast<AETVGameModeBase>(GetWorld()->GetAuthGameMode());
	PositionInWorld = GameMode->GetPosition(NewX, NewY);
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
	if (ContextMenuClass != nullptr && !IsContextMenuOpen && !PlayerController->IsPaused()
		&& !GameMode->IsTargeting() && GameMode->IsTileVisible(GetTilePosition()))
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

	// Move all Weapons actors on this Ship
	for (UETVWeaponSlot* w : Weapons)
	{
		w->GetWeapon()->GetRenderComponent()->SetMobility(EComponentMobility::Movable);
		w->GetWeapon()->SetActorLocation(GameMode->GetPosition(NewX, NewY));
	}

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

FString AETVShip::GetShipClass()
{
	if (Class == EETVShipClass::Capital)
		return "Capital";
	else if (Class == EETVShipClass::Fighter)
		return "Fighter";
	else if (Class == EETVShipClass::Repair)
		return "Repair";
	return "";
}

FString AETVShip::GetShipName()
{
	return Name;
}

void AETVShip::SpawnWeapons()
{
	// Actor spawn parameters
	const FActorSpawnParameters SpawnInfo;

	// Rotate upwards to face the top-down camera
	const FRotator Rotator(0, 0, -90);

	// Add Laser
	UETVWeaponSlot* WeaponSlotLaser = NewObject<UETVWeaponSlot>();
	WeaponSlotLaser->Init(Level, Level);

	AETVWeaponLaser* Laser;
	Laser = GetWorld()->SpawnActor<AETVWeaponLaser>(PositionInWorld, Rotator, SpawnInfo);

	do {
		Laser->InitRandom("Laser", Level);
	} while (!WeaponSlotLaser->DoesWeaponFit(Laser));
	WeaponSlotLaser->FitWeapon(Laser);
	AddWeapon(WeaponSlotLaser);
}
