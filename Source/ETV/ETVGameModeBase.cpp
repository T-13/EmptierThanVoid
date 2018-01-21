// Copyright (C) Team13. All rights reserved.

#include "ETVGameModeBase.h"
#include "ETVActionTarget_Fire.h"
#include "ETVActionTarget_Move.h"
#include "ETVActionTarget_Use.h"
#include "ETVCameraDirector.h"
#include "WidgetLayoutLibrary.h"
#include "UserWidget.h"
#include "ETVCalculator.h"
#include "Runtime/Engine/Classes/Engine/UserInterfaceSettings.h"
#include "Runtime/Core/Public/Misc/FileHelper.h"
#include "ETVAI.h"
//#include "DrawDebugHelpers.h" // Uncomment for debug drawing

// Sets default values
AETVGameModeBase::AETVGameModeBase()
{
	// Set this actor to call Tick() every frame
	PrimaryActorTick.bCanEverTick = true;

	/* Generation */
	TileSize = 32;
	MapWidth = 50.0f;
	MapHeight = 50.0f;


	/* Game Loop */
	// Set first turn to player
	CurrentTurnSide = EETVShipType::PlayerShip;

	// Disable game time (until everything is generated)
	ElapsedTime = -1.0f;
	CurrentTurn = 0;

	// One turn lasts 45 seconds
	TurnTime = 45;

	bDisableAI = false;


	/* Targeting */
	bTargeting = false;
	bTargetingOnStart = false;


	/* Visibility */
	bIgnoreVisibility = false;
	bShowEnemyVisibility = false;


	static ConstructorHelpers::FObjectFinder<UPaperTileSet> EnemyCapitalTile(TEXT("PaperTileSet'/Game/EmptierThanVoid/Art/Ships/EnemyCapitalShipTile.EnemyCapitalShipTile'"));
	EnemyCapitalShip = EnemyCapitalTile.Object;

	static ConstructorHelpers::FObjectFinder<UPaperTileSet> PlayerCapitalTile(TEXT("PaperTileSet'/Game/EmptierThanVoid/Art/Ships/PlayerCapitalShipTile.PlayerCapitalShipTile'"));
	PlayerCapitalShip = PlayerCapitalTile.Object;

	static ConstructorHelpers::FObjectFinder<UPaperTileSet> EnemyFighterTile(TEXT("PaperTileSet'/Game/EmptierThanVoid/Art/Ships/EnemyFighterShipTile.EnemyFighterShipTile'"));
	EnemyFighterShip = EnemyFighterTile.Object;

	static ConstructorHelpers::FObjectFinder<UPaperTileSet> PlayerFighterTile(TEXT("PaperTileSet'/Game/EmptierThanVoid/Art/Ships/PlayerFighterShipTile.PlayerFighterShipTile'"));
	PlayerFighterShip = PlayerFighterTile.Object;

	static ConstructorHelpers::FObjectFinder<UPaperTileSet> PlayerRepairShipTile(TEXT("PaperTileSet'/Game/EmptierThanVoid/Art/Ships/PlayerRepairShipTile.PlayerRepairShipTile'"));
	PlayerRepairShip = PlayerRepairShipTile.Object;

	static ConstructorHelpers::FObjectFinder<UPaperTileSet> EnemyRepairShipTile(TEXT("PaperTileSet'/Game/EmptierThanVoid/Art/Ships/EnemyRepairShipTile.EnemyRepairShipTile'"));
	EnemyRepairShip = EnemyRepairShipTile.Object;
}

// Called when the game starts or when spawned
void AETVGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	// Spawn Widget for Action Log
	ActionLogClass = CreateWidget<UETVActionLogWidget>(GetWorld(), ActionLogWidget);
	FVector2D VeiwportSize = UWidgetLayoutLibrary::GetViewportSize(GetWorld());

	// We need to floor the float values of the viewport size so we can pass those into the GetDPIScaleBasedOnSize function
	int32 XOfViewport = FGenericPlatformMath::FloorToInt(VeiwportSize.X);
	int32 YOfViewport = FGenericPlatformMath::FloorToInt(VeiwportSize.Y);
	float DpiScale = GetDefault<UUserInterfaceSettings>(UUserInterfaceSettings::StaticClass())->GetDPIScaleBasedOnSize(FIntPoint(XOfViewport, YOfViewport));

	// Marginand DesiredHeight for UI Widgets
	int32 MarginY = (VeiwportSize.Y * 0.03) / DpiScale;
	int32 MarginX = (VeiwportSize.X * 0.1) / DpiScale / 4; // Calculate the maximum possible margin to counteract the possibility of the widgets overlapping
	int32 DesiredHeight = 165;

	// Define the size of the element
	int32 WidthOfActionLog = (VeiwportSize.X * 0.4) / DpiScale; // We always want the Log Widget to be 40% of the screen width
	int32 HeightOfActionLog = DesiredHeight;

	// Set the location to bottom left corner
	VeiwportSize.X = (VeiwportSize.X / DpiScale) - WidthOfActionLog - MarginX;
	VeiwportSize.Y = (VeiwportSize.Y / DpiScale) - HeightOfActionLog - MarginY;

	ActionLogClass->SetPositionInViewport(VeiwportSize, false);

	ActionLogClass->SetDesiredSizeInViewport(FVector2D(WidthOfActionLog, HeightOfActionLog));
	ActionLogClass->AddToViewport();


	if (TileSet != nullptr)
	{
		MapGeneration();

		// Debug start with targeting enabled
		if (bTargetingOnStart)
		{
			bTargeting = true;
		}

		GenerateShips();

		// Spawn ShipStatus UI
		ShipStatusUI = CreateWidget<UETVShipStatusUIWidget>(GetWorld(), ShipStatusUIClass);
		// Pass ships to the ShipStatus UI
		ShipStatusUI->AssignShips(Ships);

		FVector2D VeiwportSize2 = UWidgetLayoutLibrary::GetViewportSize(GetWorld());
		// Define the size of the element
		int32 WidthOfShipStatusUI = VeiwportSize2.X*0.5 / DpiScale; // We always want the ShipSatusList to be 50% of the screen
		int32 HeightOfShipStatusUI = DesiredHeight;

		// Set the location to bottom left corner	
		VeiwportSize2.X = MarginX;
		VeiwportSize2.Y = (VeiwportSize2.Y / DpiScale) - HeightOfShipStatusUI - MarginY;
		ShipStatusUI->SetPositionInViewport(VeiwportSize2, false);
		// Resize to correct size
		ShipStatusUI->SetDesiredSizeInViewport(FVector2D(WidthOfShipStatusUI, HeightOfShipStatusUI));

		ShipStatusUI->AddToViewport();

		// Set starting visibility
		UpdateVisibleTiles(EETVShipType::PlayerShip);

		// Initialize ship list
		GetShipListWidget()->Update();

		// Start game lopp
		ElapsedTime = 0.0f;
		CurrentTurnTime = static_cast<float>(TurnTime);
	}
}

// Called every frame
void AETVGameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Game loop
	if (ElapsedTime >= 0.0f)
	{
		ElapsedTime += DeltaTime;

		if (CurrentTurnTime > 0.0f)
		{
			CurrentTurnTime -= DeltaTime;
		}
		else
		{
			EndTurn();
		}
	}

	// Targeting
	if (bTargeting && SelectedAction != nullptr)
	{
		FETVTile MouseOverTile;
		GetMouseOverTile(MouseOverTile);

		bool bIsDifferentTile = MouseOverTile.X != LastTile.X || MouseOverTile.Y != LastTile.Y;
		// Only continue if hoevered tile is different than in previous frame and we are still in targeting
		if (bIsDifferentTile && bTargeting)
		{
			CurrentTile = MouseOverTile;

			// Highlight currently hovered tile based on ability to perform an action
			FPaperTileInfo TileInfo;
			if (TileSetTarget != nullptr)
			{
				TileInfo.TileSet = TileSetTarget;
			}

			// Get ship on currently hovered tile
			AActor* TargetActor = GetShipActor(CurrentTile.X, CurrentTile.Y);
			// Get tile if no ship on that location
			if (TargetActor == nullptr)
			{
				TargetActor = TileMapActor;
			}

			// Set target
			SelectedAction->SetTarget(TargetActor, CurrentTile.X, CurrentTile.Y);

			TileInfo.PackedTileIndex = SelectedAction->CanPerform() ? EETVTargetValidity::Valid : EETVTargetValidity::Invalid;
			//TileInfo.PackedTileIndex = FMath::RandRange(0, 1); // Debug
			//TileInfo.PackedTileIndex = EETVTargetValidity::Valid; // Debug
			CurrentTile.Index = TileInfo.PackedTileIndex;
			TileMapComp->SetTile(CurrentTile.X, CurrentTile.Y, EETVTileLayer::Target, TileInfo);

			// Reset previous tile (only if tile not in confirmation phase)
			if (LastTile.IsValid() && ConfirmationTile != LastTile)
			{
				FPaperTileInfo LastTileInfo;
				LastTileInfo.PackedTileIndex = LastTile.Index;
				TileMapComp->SetTile(LastTile.X, LastTile.Y, EETVTileLayer::Target, LastTileInfo);
			}

			// Set previous tile variables to currently hovered tile
			LastTile.Set(CurrentTile);
		}
	}
}

void AETVGameModeBase::MapGeneration()
{
	// Vector for spawn location (takes into account height, width and tile size)
	const FVector LocDim(-(MapHeight * TileSize / 2), -(MapWidth * TileSize / 2), 0);

	// Actor spawn parameters
	const FActorSpawnParameters SpawnInfo;

	// Rotate upwards to face the top-down camera
	const FRotator Rotator(0, 0, -90);

	// Spawning Tile Map Actor
	TileMapActor = GetWorld()->SpawnActor<APaperTileMapActor>(LocDim, Rotator, SpawnInfo);

	// Getting Tile Map Component from Tile Map Actor
	TileMapComp = TileMapActor->GetRenderComponent();

	// Create map and make it editable (last layer)
	TileMapComp->CreateNewTileMap(MapWidth, MapHeight, 32, 32, 1.0f, false);
	TileMapComp->TileMap->SeparationPerLayer = 0.1f;
	TileMapComp->MakeTileMapEditable();

	// Create additional layers before creating board layer (which creates empty layer at the bottom)
	TileMapComp->AddNewLayer(); // Layer 3 for Targeting
	TileMapComp->AddNewLayer(); // Layer 2 for Effects
	TileMapComp->AddNewLayer(); // Layer 1 for Ships
	TileMapComp->AddNewLayer(); // Layer 0 for Board

	// Paper tile info for SetTile function
	FPaperTileInfo TileInfo;

	// This is referenced in blueprint
	TileInfo.TileSet = TileSet;

	// For filling in Tiles
	for (int32 i = 0; i < MapHeight; i++) {
		for (int32 j = 0; j < MapWidth; j++) {

			// PackedTileIndex is -1 by default
			TileInfo.PackedTileIndex = 0;

			// Setting the tile with coordinates i and j
			TileMapComp->SetTile(i, j, EETVTileLayer::Board, TileInfo);

			// Add to Tile Data
			FETVTileData CurrentTileData;
			CurrentTileData.Tile.Set(j, i, TileInfo.PackedTileIndex);

			TArray<FVector> CurrentTilePoints;
			TileMapComp->GetTilePolygon(j, i, CurrentTilePoints, 0, true);
			CurrentTileData.PointLeftTop = FVector2D(CurrentTilePoints[0]);
			CurrentTileData.PointRightBottom = FVector2D(CurrentTilePoints[2]);

			TileData.Add(CurrentTileData);
		}
	}

	// Throw event when done
	MapGenerated(TileMapActor);

	// Bind click
	UInputComponent* InputComponent = GetWorld()->GetFirstPlayerController()->InputComponent;
	InputComponent->BindAction("Select Tile", IE_Pressed, this, &AETVGameModeBase::OnClickedSelectMapTile);
	InputComponent->BindAction("Select Tile", IE_Released, this, &AETVGameModeBase::OnReleasedSelectMapTile);
	InputComponent->BindAction("Cancel Targeting", IE_Pressed, this, &AETVGameModeBase::OnClickedCancelTargeting);
}

void AETVGameModeBase::GenerateShips()
{
	// Array for names of Ships
	TArray<FString> CapitalShipNames;
	TArray<FString> FighterShipNames;
	TArray<FString> RepairShipNames;

	// All names for Capital Ships to array CapitalShipNames
	FString ProjectDir = FPaths::ProjectDir();
	FString CompleteFilePath = ProjectDir + "Content/EmptierThanVoid/Core/Ship Names/CapitalShipNames.txt";
	FString Names = "";
	FFileHelper::LoadFileToString(Names, *CompleteFilePath);
	Names.ParseIntoArray(CapitalShipNames, TEXT("\n"), false);

	// All names for Fighter Ships to array FighterShipNames
	CompleteFilePath = ProjectDir + "Content/EmptierThanVoid/Core/Ship Names/FighterShipNames.txt";
	Names = "";
	FFileHelper::LoadFileToString(Names, *CompleteFilePath);
	Names.ParseIntoArray(FighterShipNames, TEXT("\n"), false);

	// All names for Repair Ships to array RepairShipNames
	CompleteFilePath = ProjectDir + "Content/EmptierThanVoid/Core/Ship Names/RepairShipNames.txt";
	Names = "";
	FFileHelper::LoadFileToString(Names, *CompleteFilePath);
	Names.ParseIntoArray(RepairShipNames, TEXT("\n"), false);

	int32 ycoord;
	int32 xcoord;
	int32 nameIndex;

	// Number of FighterShips Spawned on each side
	int32 numOfFighters = FMath::FRandRange(FMath::Sqrt(MapWidth), MapWidth / 2);

	// Number of RepairShips Spawned on each side (1 one each side for every 10 Ships overall)
	int32 numOfRepairShips = ((numOfFighters * 2 + 2) / 10) + 1;

	FPaperTileInfo TileInfo;
	TileInfo.PackedTileIndex = 0;

	// Spawning Capital Ship for Player
	TileInfo.TileSet = PlayerCapitalShip;
	ycoord = FMath::FRandRange(0, MapWidth);
	xcoord = 0;
	TileMapComp->SetTile(xcoord, ycoord, EETVTileLayer::Ship, TileInfo);

	// Name of Ship
	nameIndex = FMath::FRandRange(0, CapitalShipNames.Num() - 1);
	SpawnShipType(xcoord, ycoord, TileInfo.TileSet, CapitalShipNames[nameIndex]);

	// Spawning Capital Ship for Enemy
	TileInfo.TileSet = EnemyCapitalShip;
	ycoord = FMath::FRandRange(0, MapWidth);
	xcoord = MapWidth - 1;
	TileMapComp->SetTile(xcoord, ycoord, EETVTileLayer::Ship, TileInfo);

	// Name of Ship
	nameIndex = FMath::FRandRange(0, CapitalShipNames.Num() - 1);
	while (IsShipNameUsed(CapitalShipNames[nameIndex])) {
		nameIndex = FMath::FRandRange(0, CapitalShipNames.Num() - 1);
	}

	SpawnShipType(xcoord, ycoord, TileInfo.TileSet, CapitalShipNames[nameIndex]);

	// Spawning Repair Ships on each side
	SpawnShipsLoop(PlayerRepairShip, EnemyRepairShip, numOfRepairShips, RepairShipNames);

	// Spawning Fighter Ships on each side
	SpawnShipsLoop(PlayerFighterShip, EnemyFighterShip, numOfFighters, FighterShipNames);

	// Balancing Levels on both sides
	int32 PlayerShipsLevel = 0;
	int32 EnemyShipsLevel = 0;
	for (AETVShip* ship : Ships) {
		if (ship->IsEnemy()) {
			EnemyShipsLevel += ship->GetLevel();
		}
		else {
			PlayerShipsLevel += ship->GetLevel();
		}
	}
	int32 LevelDiff = PlayerShipsLevel - EnemyShipsLevel;

	// If level difference is larger than 100
	while (FMath::Abs(LevelDiff) > 100) {
		AETVShip* PlayerShip = Ships[3];
		AETVShip* EnemyShip = Ships[4];
		if (PlayerShipsLevel > EnemyShipsLevel) {

			// Finding Player ship with biggest level and Enemy ship with smallest level
			for (AETVShip* ship : Ships) {
				if (ship->GetClass() != EETVShipClass::Capital) {
					if (!ship->IsEnemy()) {
						if (ship->GetLevel() > PlayerShip->GetLevel()) {
							PlayerShip = ship;
						}
					}
					else {
						if (ship->GetLevel() < EnemyShip->GetLevel()) {
							EnemyShip = ship;
						}
					}
				}
			}

			// New Init with +-50 level
			PlayerShip->InitRandomWithLevel(PlayerShip->GetShipName(), PlayerShip->GetLevel() - 50);
			EnemyShip->InitRandomWithLevel(EnemyShip->GetShipName(), EnemyShip->GetLevel() + 50);
		}
		else {

			// Finding Enemy ship with biggest level and Player ship with smallest level
			for (AETVShip* ship : Ships) {
				if (ship->GetClass() != EETVShipClass::Capital) {
					if (!ship->IsEnemy()) {
						if (ship->GetLevel() < PlayerShip->GetLevel()) {
							PlayerShip = ship;
						}
					}
					else {
						if (ship->GetLevel() > EnemyShip->GetLevel()) {
							EnemyShip = ship;
						}
					}
				}
			}
			// New Init with +-50 level
			PlayerShip->InitRandomWithLevel(PlayerShip->GetShipName(), PlayerShip->GetLevel() + 50);
			EnemyShip->InitRandomWithLevel(EnemyShip->GetShipName(), EnemyShip->GetLevel() - 50);
		}

		// Calculating new level difference
		PlayerShipsLevel = 0;
		EnemyShipsLevel = 0;
		for (AETVShip* ship : Ships) {
			if (ship->IsEnemy()) {
				EnemyShipsLevel += ship->GetLevel();
			}
			else {
				PlayerShipsLevel += ship->GetLevel();
			}
		}
		LevelDiff = PlayerShipsLevel - EnemyShipsLevel;
	}
}

void AETVGameModeBase::SpawnShipsLoop(UPaperTileSet * Player, UPaperTileSet * Enemy, int32 NumOfShips, TArray<FString> ShipNames)
{
	int32 xcoord, ycoord, nameIndex;
	FPaperTileInfo TileInfo;
	TileInfo.PackedTileIndex = 0;

	for (int32 i = 0; i < NumOfShips; i++) {
		// Player Fighter Ship
		TileInfo.TileSet = Player;

		ycoord = FMath::FRandRange(0, MapWidth);
		// So that middle 20% are left empty
		xcoord = FMath::FRandRange(0, (MapWidth / 2 - MapWidth * 0.1));

		// Loop for looking for empty Tile
		while (TileHasShip(xcoord, ycoord)) {
			ycoord = FMath::FRandRange(0, MapWidth);
			// So that middle 20% are left empty
			xcoord = FMath::FRandRange(0, (MapWidth / 2 - MapWidth * 0.1));
		}

		// Set Tile
		TileMapComp->SetTile(xcoord, ycoord, EETVTileLayer::Ship, TileInfo);

		// Name of Ship
		nameIndex = FMath::FRandRange(0, ShipNames.Num() - 1);

		while (IsShipNameUsed(ShipNames[nameIndex])) {
			nameIndex = FMath::FRandRange(0, ShipNames.Num() - 1);
		}
		SpawnShipType(xcoord, ycoord, TileInfo.TileSet, ShipNames[nameIndex]);


		// Enemy Fighter Ship
		TileInfo.TileSet = Enemy;

		ycoord = FMath::FRandRange(0, MapWidth);
		// So that middle 20% are left empty
		xcoord = FMath::FRandRange((MapWidth / 2 + MapWidth * 0.1), MapWidth);

		// Loop for looking for empty Tile
		while (TileHasShip(xcoord, ycoord)) {
			ycoord = FMath::FRandRange(0, MapWidth);
			// So that middle 20% are left empty
			xcoord = FMath::FRandRange((MapWidth / 2 + MapWidth * 0.1), MapWidth);
		}

		// Set Tile
		TileMapComp->SetTile(xcoord, ycoord, EETVTileLayer::Ship, TileInfo);

		// Name of Ship
		nameIndex = FMath::FRandRange(0, ShipNames.Num() - 1);
		while (IsShipNameUsed(ShipNames[nameIndex])) {
			nameIndex = FMath::FRandRange(0, ShipNames.Num() - 1);
		}

		SpawnShipType(xcoord, ycoord, TileInfo.TileSet, ShipNames[nameIndex]);
	}
}

void AETVGameModeBase::SpawnShipType(int32 x, int32 y, UPaperTileSet* type, FString name)
{
	if (type == PlayerCapitalShip)
		SpawnShip<AETVShipCapital>(x, y, name, false);
	else if (type == EnemyCapitalShip)
		SpawnShip<AETVShipCapital>(x, y, name, true);
	else if (type == PlayerFighterShip)
		SpawnShip<AETVShipFighter>(x, y, name, false);
	else if (type == EnemyFighterShip)
		SpawnShip<AETVShipFighter>(x, y, name, true);
	else if (type == PlayerRepairShip)
		SpawnShip<AETVShipRepairShip>(x, y, name, false);
	else if (type == EnemyRepairShip)
		SpawnShip<AETVShipRepairShip>(x, y, name, true);
}

void AETVGameModeBase::SpawnActions(AETVShip* Ship)
{
	for (UETVWeaponSlot* w : Ship->GetWeapons())
	{
		UETVActionTarget *Fire;
		if (w->GetWeapon()->GetType() == AETVWeapon::HealHull || w->GetWeapon()->GetType() == AETVWeapon::HealShield)
		{
			Fire = NewObject<UETVActionTarget_Use>();
		}
		else
		{
			Fire = NewObject<UETVActionTarget_Fire>();
		}
		Fire->Init(Ship, w->GetWeapon());
		Ship->AddAction(Fire);
	}

	UETVActionTarget_Move *Move = NewObject<UETVActionTarget_Move>();
	Move->Init(Ship);
	Ship->AddAction(Move);
}

AETVShip* AETVGameModeBase::GetShipActor(int32 x, int32 y)
{
	const FVector LocDim = GetPosition(x, y);

	// Iterate through Ships TArray to find the correct UETVShip
	for (AETVShip* ship : Ships) {
		if (ship->GetActorLocation() == LocDim) {
			return ship;
		}
	}

	return nullptr;
}

FVector AETVGameModeBase::GetPosition(int32 x, int32 y, float z)
{
	return FVector(-(TileSize / 2)*MapWidth + x * TileSize, -(TileSize / 2)*MapHeight + y * TileSize, z);
}

void AETVGameModeBase::SetPosition(int32 ToX, int32 ToY, int32 FromX, int32 FromY)
{
	// Set new tile
	FPaperTileInfo ToTileInfo = TileMapComp->GetTile(FromX, FromY, EETVTileLayer::Ship);
	TileMapComp->SetTile(ToX, ToY, EETVTileLayer::Ship, ToTileInfo);

	// Reset old tile
	TileMapComp->SetTile(FromX, FromY, EETVTileLayer::Ship, FPaperTileInfo());
}

void AETVGameModeBase::DestroyShip(AETVShip * DestroyedShip)
{
	// Remove Ship from TArray and update ShipStatus UI
	Ships.Remove(DestroyedShip);
	ShipStatusUI->AssignShips(Ships);
	GetShipListWidget()->Update();

	// Reset Tile
	FPaperTileInfo TileInfo;
	TileInfo.TileSet = nullptr;
	TileMapComp->SetTile(DestroyedShip->GetX(), DestroyedShip->GetY(), EETVTileLayer::Ship, TileInfo);

	// Remove Actor
	DestroyedShip->Destroy();
}

void AETVGameModeBase::EndTurn()
{
	// Close context menu if any is open
	// TODO Optimize by closing the only open context menu as there can't be more than one open
	for (AETVShip* CurShip : Ships)
	{
		CurShip->UnconditionallyCloseContextMenu();
	}

	CurrentTurnSide = EETVShipType::EnemyShip;
	CurrentTurnTime = 0.0f;

	// Handle turn end
	if (bDisableAI)
	{
		// Continue to next turn directly
		NextTurn();
	}
	else
	{
		// Pause for AI
		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
		PlayerController->SetPause(true);

		if (bShowEnemyVisibility)
		{
			UpdateVisibleTiles(EETVShipType::EnemyShip);
		}

		// Move control to AI
		UETVAI* AI = NewObject<UETVAI>();
		TArray<int32> Instructions = AI->GetMove(Ships);
		if (Instructions.Num() == 4)
		{
			UETVActionTarget* Action = Cast<UETVActionTarget>(Ships[Instructions[1]]->GetActions()[Instructions[2]]);
			Action->SetTarget(Ships[Instructions[3]], Ships[Instructions[3]]->GetX(), Ships[Instructions[3]]->GetY());
			Action->Perform();
		}
		else if (Instructions.Num() == 5)
		{
			UETVActionTarget* Action = Cast<UETVActionTarget>(Ships[Instructions[1]]->GetActions()[Instructions[2]]);
			Action->SetTarget(TileMapActor, Instructions[3], Instructions[4]);
			Action->Perform();
		}
		
		// AI continue to next turn when done
		NextTurn();
	}
}

void AETVGameModeBase::NextTurn()
{
	// Update visibility (eg. player ships have been destroyed)
	UpdateVisibleTiles(EETVShipType::PlayerShip);

	// Update ship list (eg. enemy ships became hidden or visible)
	GetShipListWidget()->Update();

	// Apply next turn
	CurrentTurnSide = EETVShipType::PlayerShip;
	CurrentTurn++;
	CurrentTurnTime = static_cast<float>(TurnTime);

	// Unpause
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	PlayerController->SetPause(false);

	// Perform multi-turn actions
	for (auto &MultiTurnAction : MultiTurnActions)
	{
		MultiTurnAction->Perform();
	}
}

float AETVGameModeBase::GetCurrentTurnPercentage()
{
	return 1.0f - CurrentTurnTime / static_cast<float>(TurnTime);
}

void AETVGameModeBase::AddMultiTurnAction(UETVAction* Action)
{
	MultiTurnActions.Add(Action);
}

void AETVGameModeBase::RemoveMultiTurnAction(UETVAction* Action)
{
	MultiTurnActions.Remove(Action);
}

bool AETVGameModeBase::IsTargeting()
{
	return bTargeting;
}

bool AETVGameModeBase::IsPositionOnTileMap(const FVector Location)
{
	bool bInRangeX = UKismetMathLibrary::InRange_FloatFloat(Location.X, TileData[0].PointLeftTop.X, TileData.Last().PointRightBottom.X, false, false);
	bool bInRangeY = UKismetMathLibrary::InRange_FloatFloat(Location.Y, TileData[0].PointLeftTop.Y, TileData.Last().PointRightBottom.Y, false, false);

	return bInRangeX && bInRangeY;
}

void AETVGameModeBase::GetMouseOverTile(FETVTile& Tile)
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

	FVector WorldLocation;
	FVector WorldDirection;
	if (PlayerController->DeprojectMousePositionToWorld(WorldLocation, WorldDirection))
	{
		AETVCameraDirector* Camera = Cast<AETVCameraDirector>(PlayerController->GetPawn());
		if (Camera == nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("GetMouseOverTile(): Pawn not set to AETVCameraDirector!"));
			return;
		}

		// Add height to tile map
		WorldDirection *= abs((Camera->GetZoom() + 10.0f) / WorldDirection.Z); // Add 10.0f due to #68 (start is 20 UU higher, but for some reason half is taken)
		WorldLocation += WorldDirection;

		// Check if on tile at all
		if (IsPositionOnTileMap(WorldLocation))
		{
			// Check each tile if in range
			for (auto &CurrentTileData : TileData)
			{
				bool bInRangeX = UKismetMathLibrary::InRange_FloatFloat(WorldLocation.X, CurrentTileData.PointLeftTop.X, CurrentTileData.PointRightBottom.X, false, false);
				bool bInRangeY = UKismetMathLibrary::InRange_FloatFloat(WorldLocation.Y, CurrentTileData.PointLeftTop.Y, CurrentTileData.PointRightBottom.Y, false, false);
				if (bInRangeX && bInRangeY)
				{
					/* Uncomment to enable visual representation of targeting (debug), "DrawDebugHelpers.h" include required
					DrawDebugPoint(GetWorld(), WorldLocation, 5.0f, FColor(255, 0, 255), false); // Current mouse position
					FVector DebugPointTileCenter = FVector(CurrentTileData.PointLeftTop.X, CurrentTileData.PointLeftTop.Y, 0.0f) * 0.5f + FVector(CurrentTileData.PointRightBottom.X, CurrentTileData.PointRightBottom.Y, 0.0f) * 0.5f;
					DrawDebugPoint(GetWorld(), DebugPointTileCenter, 5.0f, FColor(255, 0, 0), false);
					DrawDebugLine(GetWorld(), WorldLocation, DebugPointTileCenter, FColor(255, 0, 0), false);
					UE_LOG(LogTemp, Warning, TEXT("Loc (%s) -- Dir (%s) -- Zoom %g"), *WorldLocation.ToString(), *WorldDirection.ToString(), Camera->GetZoom());
					*/

					Tile.Set(CurrentTileData.Tile);
					return;
				}
			}
		}
	}

	Tile.Invalidate();
}

void AETVGameModeBase::OnClickedSelectMapTile()
{
	// Make sure we are still targeting (in case of race conditions)
	if (bTargeting && !PreDelayTile.IsValid() && TileSetTargetClick != nullptr)
	{
		PreDelayTile.Set(CurrentTile); // Prevents race conditions when leaving tile between click and release

		// Set Tile Set to Click
		FPaperTileInfo TileInfo = TileMapComp->GetTile(PreDelayTile.X, PreDelayTile.Y, EETVTileLayer::Target);
		TileInfo.TileSet = TileSetTargetClick;
		TileMapComp->SetTile(PreDelayTile.X, PreDelayTile.Y, EETVTileLayer::Target, TileInfo);

		// Stop targeting if valid target
		if (PreDelayTile.Index == EETVTargetValidity::Valid)
		{
			// Reset previous tile
			FPaperTileInfo LastTileInfo;
			LastTileInfo.PackedTileIndex = ConfirmationTile.Index;
			TileMapComp->SetTile(ConfirmationTile.X, ConfirmationTile.Y, EETVTileLayer::Target, LastTileInfo);

			if (ConfirmationTile.IsValid() && PreDelayTile == ConfirmationTile)
			{
				ConfirmationTile.Invalidate();
				StopTargeting();
			}
			else
			{
				ConfirmationTile.Set(CurrentTile);
			}
		}
	}
}

void AETVGameModeBase::OnReleasedSelectMapTile()
{
	if (PreDelayTile.IsValid())
	{
		// Remove highlight Tile Set
		FPaperTileInfo ResetTileInfo;
		ResetTileInfo.PackedTileIndex = PreDelayTile.Index;
		if (PreDelayTile == CurrentTile && TileSetTarget != nullptr)
		{
			ResetTileInfo.TileSet = TileSetTarget;
		}
		TileMapComp->SetTile(PreDelayTile.X, PreDelayTile.Y, EETVTileLayer::Target, ResetTileInfo);

		// Set pre-delay tile variable to invalid
		PreDelayTile.Invalidate();
	}
}

void AETVGameModeBase::OnClickedCancelTargeting()
{
	StopTargeting(false);
}

void AETVGameModeBase::StartTargeting(UETVActionTarget* Action)
{
	SelectedAction = Action;

	bTargeting = true;
}

void AETVGameModeBase::StopTargeting(bool bSuccess)
{
	bTargeting = false;

	// Reset current and confirmation (if it exists) tiles
	FPaperTileInfo LastTileInfo;
	LastTileInfo.PackedTileIndex = CurrentTile.Index;
	TileMapComp->SetTile(CurrentTile.X, CurrentTile.Y, EETVTileLayer::Target, LastTileInfo);

	if (ConfirmationTile.IsValid())
	{
		LastTileInfo.PackedTileIndex = ConfirmationTile.Index;
		TileMapComp->SetTile(ConfirmationTile.X, ConfirmationTile.Y, EETVTileLayer::Target, LastTileInfo);
	}

	// Set previous tile variables to invalid
	LastTile.Invalidate();
	CurrentTile.Invalidate();

	// Make sure nothing stopped targeting
	if (bSuccess && SelectedAction != nullptr)
	{
		// Double check that if we can perform, then perform
		if (SelectedAction->CanPerform())
		{
			SelectedAction->Perform();
		}
	}

	SelectedAction = nullptr;
}

float AETVGameModeBase::GetTiledDistance(FVector2D TileA, FVector2D TileB)
{
	// Calculate distance between points
	float Distance = sqrtf(powf(TileB.X - TileA.X, 2) + powf(TileB.Y - TileA.Y, 2));

	// Round up (tile distance)
	return floorf(Distance);
}

void AETVGameModeBase::UpdateVisibleTiles(EETVShipType Side, TArray<FVector2D>& VisibleTiles)
{
	// Go through all tiles checking if they are visible by any ship on given side
	for (auto &CurrentTileData : TileData)
	{
		FVector2D CurrentTile = FVector2D(CurrentTileData.Tile.X, CurrentTileData.Tile.Y);
		bool bIsVisible = IsTileVisible(CurrentTile, Side);

		if (bIsVisible)
		{
			VisibleTiles.Add(CurrentTile);
		}

		// Update effects if called for player
		if (Side == EETVShipType::PlayerShip || bShowEnemyVisibility)
		{
			SetTileVisibilityEffect(CurrentTile.X, CurrentTile.Y, bIsVisible || bIgnoreVisibility);
		}
	}
}

void AETVGameModeBase::UpdateVisibleTiles(EETVShipType Side)
{
	TArray<FVector2D> VisibleTilesDummy;
	UpdateVisibleTiles(Side, VisibleTilesDummy);
}

void AETVGameModeBase::GetVisibleShips(EETVShipType Side, TArray<AETVShip*>& VisibleShips)
{
	VisibleShips = Ships.FilterByPredicate([](AETVShip* Ship) {
		return Ship->IsVisible();
	});
}

bool AETVGameModeBase::IsTileVisible(FVector2D Tile, EETVShipType Side)
{
	bool bIsVisible = false;
	// Check visibility from each ship
	for (auto &CurrentShip : Ships)
	{
		// Skip if not wanted side
		if (CurrentShip->GetType() != Side)
		{
			continue;
		}

		// Check if in neighbouring tile (always visible) or in sensor range
		float Distance = GetTiledDistance(CurrentShip->GetTilePosition(), Tile);
		if (Distance <= 1 || Distance <= CurrentShip->GetSensorRange())
		{
			bIsVisible = true;
			break;
		}
	}

	return bIsVisible;
}

void AETVGameModeBase::SetTileVisibilityEffect(int32 X, int32 Y, bool bVisible)
{
	if (bVisible)
	{
		TileMapComp->SetTile(X, Y, EETVTileLayer::Effect, FPaperTileInfo());
	}
	else
	{
		FPaperTileInfo TileInfo;
		TileInfo.TileSet = TileSetHidden;
		TileInfo.PackedTileIndex = 0;
		TileMapComp->SetTile(X, Y, EETVTileLayer::Effect, TileInfo);
	}
}

void AETVGameModeBase::ShipClicked(AETVShip *ClickedShip)
{
	LastClickedShip = ClickedShip;
}

bool AETVGameModeBase::WasShipClickedRecently()
{
	return LastClickedShip != nullptr;
}

bool AETVGameModeBase::IsShipNameUsed(FString Name)
{
	for (AETVShip* ship : Ships) {
		if (ship->GetShipName().Equals(Name))
			return true;
	}
	return false;
}

bool AETVGameModeBase::TileHasShip(int32 x, int32 y)
{
	FPaperTileInfo TileInfo = TileMapComp->GetTile(x, y, EETVTileLayer::Ship);
	return TileInfo.TileSet != nullptr;
}

UPaperTileSet* AETVGameModeBase::GetShipSprite(AETVShip* Ship)
{
	return TileMapComp->GetTile(Ship->GetX(), Ship->GetY(), EETVTileLayer::Ship).TileSet;
}
