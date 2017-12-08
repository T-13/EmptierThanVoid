// Copyright (C) Team13. All rights reserved.

#include "ETVGameModeBase.h"
#include "ETVWeaponLaser.h"
#include "ETVWeaponTorpedo.h"
#include "ETVWeaponRepairArm.h"
#include "ETVWeaponShieldBattery.h"

// Sets default values
AETVGameModeBase::AETVGameModeBase()
{
	// Set this actor to call Tick() every frame
	PrimaryActorTick.bCanEverTick = true;

	/* Generation */
	TileSize = 32;
	MapWidth = 25.0f;
	MapHeight = 25.0f;


	/* Game Loop */
	// Disable game time (until everything is generated)
	ElapsedTime = -1.0f;
	CurrentTurn = 0;

	// One turn lasts 45 seconds
	TurnTime = 45;

	bDisableAI = false;


	/* Targeting */
	bTargeting = false;
	bTargetingOnStart = false;

	static ConstructorHelpers::FObjectFinder<UPaperTileSet> EnemyCapitalTile(TEXT("PaperTileSet'/Game/EmptierThanVoid/Art/Ships/EnemyCapitalShipTile.EnemyCapitalShipTile'"));
	EnemyCapitalShip = EnemyCapitalTile.Object;

	static ConstructorHelpers::FObjectFinder<UPaperTileSet> PlayerCapitalTile(TEXT("PaperTileSet'/Game/EmptierThanVoid/Art/Ships/PlayerCapitalShipTile.PlayerCapitalShipTile'"));
	PlayerCapitalShip = PlayerCapitalTile.Object;

	static ConstructorHelpers::FObjectFinder<UPaperTileSet> EnemyFighterTile(TEXT("PaperTileSet'/Game/EmptierThanVoid/Art/Ships/EnemyFighterShipTile.EnemyFighterShipTile'"));
	EnemyFighterShip = EnemyFighterTile.Object;

	static ConstructorHelpers::FObjectFinder<UPaperTileSet> PlayerFighterTile(TEXT("PaperTileSet'/Game/EmptierThanVoid/Art/Ships/PlayerFighterShipTile.PlayerFighterShipTile'"));
	PlayerFighterShip = PlayerFighterTile.Object;
}

// Called when the game starts or when spawned
void AETVGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	if (TileSet != nullptr)
	{
		MapGeneration();

		// Debug start with targeting enabled
		if (bTargetingOnStart)
		{
			bTargeting = true;
		}

		GenerateShips();

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
	if (bTargeting)
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
			TileInfo.PackedTileIndex = SelectedAction != nullptr && SelectedAction->CanPerform() ? EETVTargetValidity::Valid : EETVTargetValidity::Invalid;
			//TileInfo.PackedTileIndex = FMath::RandRange(0, 1); // Debug
			CurrentTile.Index = TileInfo.PackedTileIndex;
			TileMapComp->SetTile(CurrentTile.X, CurrentTile.Y, EETVTileLayer::Target, TileInfo);

			// Reset previous tile (only if not in click phase)
			if (LastTile.IsValid())
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
	TileHeight = -450; // TODO Scale dynamically based on size
	const FVector LocDim(-(MapHeight * TileSize / 2), -(MapWidth * TileSize / 2), TileHeight);

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
	TileMapComp->AddNewLayer(); // Layer 3 for Effects
	TileMapComp->AddNewLayer(); // Layer 2 for Ships
	TileMapComp->AddNewLayer(); // Layer 1 for Targeting
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
	InputComponent->BindAction("ClickTile", IE_Pressed, this, &AETVGameModeBase::OnClickedMapTile);
	InputComponent->BindAction("ClickTile", IE_Released, this, &AETVGameModeBase::OnReleasedMapTile);
}

void AETVGameModeBase::GenerateShips()
{
	int32 ycoord;
	int32 xcoord;
	int32 numOfSpawnedShips = FMath::FRandRange(FMath::Sqrt(MapWidth), MapWidth / 2);

	// To check if Tile is allready set
	bool bIsTileSet;

	// Array for all set x coordinates
	TArray<int32> xArr;

	// Array for all set y coordinates
	TArray<int32> yArr;

	FPaperTileInfo TileInfo;
	TileInfo.PackedTileIndex = 0;

	// Spawning Capital Ship for Player
	TileInfo.TileSet = PlayerCapitalShip;
	ycoord = FMath::FRandRange(0, MapWidth);
	xcoord = 0;
	TileMapComp->SetTile(xcoord, ycoord, EETVTileLayer::Ship, TileInfo);

	// So that nothing will overlay the Capital
	xArr.Push(0);
	yArr.Push(ycoord);

	SpawnShip(xcoord, ycoord, TileInfo.TileSet);


	// Spawning Capital Ship for Enemy
	TileInfo.TileSet = EnemyCapitalShip;
	ycoord = FMath::FRandRange(0, MapWidth);
	xcoord = MapWidth - 1;
	TileMapComp->SetTile(xcoord, ycoord, EETVTileLayer::Ship, TileInfo);

	// So that nothing will overlay the Capital
	xArr.Push(MapWidth - 1);
	yArr.Push(ycoord);

	SpawnShip(xcoord, ycoord, TileInfo.TileSet);

	// Spawning Fighter Ships on each side
	for (int32 i = 0; i < numOfSpawnedShips; i++) {

		bIsTileSet = true;
		// Player Fighter Ship
		TileInfo.TileSet = PlayerFighterShip;

		// Loop for looking for empty Tile
		while (bIsTileSet) {
			bIsTileSet = false;
			ycoord = FMath::FRandRange(0, MapWidth);

			// So that middle 20% are left empty
			xcoord = FMath::FRandRange(0, (MapWidth / 2 - MapWidth * 0.1));

			for (int j = 0; j < xArr.Num(); j++) {
				if (xArr[j] == xcoord && yArr[j] == ycoord) {
					bIsTileSet = true;
				}
			}
		}

		// Set Tile
		TileMapComp->SetTile(xcoord, ycoord, EETVTileLayer::Ship, TileInfo);

		// Push to array for later to check if those coordinates are allready filled
		xArr.Push(xcoord);
		yArr.Push(ycoord);

		SpawnShip(xcoord, ycoord, TileInfo.TileSet);


		// Enemy Fighter Ship
		TileInfo.TileSet = EnemyFighterShip;

		bIsTileSet = true;

		// Loop for looking for empty Tile
		while (bIsTileSet) {
			bIsTileSet = false;
			ycoord = FMath::FRandRange(0, MapWidth);

			// So that middle 20% are left empty
			xcoord = FMath::FRandRange((MapWidth / 2 + MapWidth * 0.1), MapWidth);

			for (int j = 0; j < xArr.Num(); j++) {
				if (xArr[j] == xcoord && yArr[j] == ycoord) {
					bIsTileSet = true;
				}
			}
		}

		// Set Tile
		TileMapComp->SetTile(xcoord, ycoord, EETVTileLayer::Ship, TileInfo);

		// Push to array for later to check if those coordinates are allready filled
		xArr.Push(xcoord);
		yArr.Push(ycoord);

		SpawnShip(xcoord, ycoord, TileInfo.TileSet);

	}
}

void AETVGameModeBase::SpawnShip(int32 x, int32 y, UPaperTileSet* type)
{
	// Vector for spawn location based on where TileSet is in TileMap
	const FVector LocDim = GetPosition(x, y);

	// Actor spawn parameters
	const FActorSpawnParameters SpawnInfo;

	// Rotate upwards to face the top-down camera
	const FRotator Rotator(0, 0, -90);

	// Spawning ShipActor based on class
	if (type == PlayerCapitalShip || type == EnemyCapitalShip) {
		CapitalShip = GetWorld()->SpawnActor<AETVShipCapital>(LocDim, Rotator, SpawnInfo);
		CapitalShip->Init("Cap", 100, 100, 100, 10, 10, 10, 10, true, 1.0f, 1);
		CapitalShip->SetContextMenu(ContextMenu);
		CapitalShip->GetRenderComponent()->SetMobility(EComponentMobility::Movable);
		CapitalShip->GetRenderComponent()->SetSprite(Sprite);
		
		// Setting sprite color to transparent
		CapitalShip->GetRenderComponent()->SetSpriteColor(FLinearColor(1.0f, 1.0f, 1.0f, 0.0f));
		Ships.Add(CapitalShip);
		if (type == EnemyCapitalShip)
			CapitalShip->SetTypeToEnemy();
		for (int32 i = 0; i < 4; i++) {
			if (i == 0)  // Laser
			{
				SpawnWeapon(x, y, CapitalShip, AETVWeapon::DamageHull);
			}
			else if (i == 1) // Torpedo
			{
				SpawnWeapon(x, y, CapitalShip, AETVWeapon::DamageShieldThenHull);
			}
			else if (i == 2) // RepairArm
			{
				SpawnWeapon(x, y, CapitalShip, AETVWeapon::HealHull);
			}
			else if (i == 3) // ShieldBattery
			{
				SpawnWeapon(x, y, CapitalShip, AETVWeapon::HealShield);
			}
		}
		CapitalShip->SetActionsForWeapons();
		Ships.Add(CapitalShip);
	}
	else if (type == PlayerFighterShip || type == EnemyFighterShip) {
		FighterShip = GetWorld()->SpawnActor<AETVShipFighter>(LocDim, Rotator, SpawnInfo);
		FighterShip->Init("Fighter", 100, 100, 100, 10, 10, 10, 10, 1.0f, 2.0f);
		FighterShip->SetContextMenu(ContextMenu);
		FighterShip->GetRenderComponent()->SetMobility(EComponentMobility::Movable);
		FighterShip->GetRenderComponent()->SetSprite(Sprite);		
		// Setting sprite color to transparent
		FighterShip->GetRenderComponent()->SetSpriteColor(FLinearColor(1.0f, 1.0f, 1.0f, 0.0f));
		if (type == EnemyFighterShip)
			FighterShip->SetTypeToEnemy();
		for (int32 i = 0; i < 3; i++) {
			if (i == 0)  // Laser
			{
				SpawnWeapon(x, y, FighterShip, AETVWeapon::DamageHull);
			}
			else if (i == 1) // Torpedo
			{
				SpawnWeapon(x, y, FighterShip, AETVWeapon::DamageShieldThenHull);
			}
			else if (i == 2) // ShieldBattery
			{
				SpawnWeapon(x, y, FighterShip, AETVWeapon::HealShield);
			}
		}
		FighterShip->SetActionsForWeapons();
		Ships.Add(FighterShip);
	}
}

void AETVGameModeBase::SpawnWeapon(int32 NewX, int32 NewY, AETVShip* Ship, int32 type)
{
	UETVWeaponSlot* WeaponSlot = NewObject<UETVWeaponSlot>();

	// Vector for spawn location based on where TileSet is in TileMap
	const FVector LocDim = GetPosition(NewX, NewY);

	// Actor spawn parameters
	const FActorSpawnParameters SpawnInfo;

	// Rotate upwards to face the top-down camera
	const FRotator Rotator(0, 0, -90);
	// Spawning ShipActor based on class
	if (type == AETVWeapon::DamageShieldThenHull)
	{
		AETVWeaponLaser* Laser;
		Laser = GetWorld()->SpawnActor<AETVWeaponLaser>(LocDim, Rotator, SpawnInfo);

		do {
			Laser->InitRandom("Laser", 100);
		} while (WeaponSlot->DoesWeaponFit(Laser));
		WeaponSlot->FitWeapon(Laser);
		Ship->AddWeapon(WeaponSlot);
	}
	else if (type == AETVWeapon::DamageHull)
	{
		AETVWeaponTorpedo* Torpedo;
		Torpedo = GetWorld()->SpawnActor<AETVWeaponTorpedo>(LocDim, Rotator, SpawnInfo);

		do {
			Torpedo->InitRandom("Torpedo", 100);
		} while (WeaponSlot->DoesWeaponFit(Torpedo));
		WeaponSlot->FitWeapon(Torpedo);
		Ship->AddWeapon(WeaponSlot);
	}
	else if (type == AETVWeapon::HealHull)
	{
		AETVWeaponRepairArm* RepairArm;
		RepairArm = GetWorld()->SpawnActor<AETVWeaponRepairArm>(LocDim, Rotator, SpawnInfo);

		do {
			RepairArm->InitRandom("Torpedo", 100);
		} while (WeaponSlot->DoesWeaponFit(RepairArm));
		WeaponSlot->FitWeapon(RepairArm);
		Ship->AddWeapon(WeaponSlot);
	}
	else if (type == AETVWeapon::HealShield)
	{
		AETVWeaponShieldBattery* ShieldBattery;
		ShieldBattery = GetWorld()->SpawnActor<AETVWeaponShieldBattery>(LocDim, Rotator, SpawnInfo);

		do {
			ShieldBattery->InitRandom("Torpedo", 100);
		} while (WeaponSlot->DoesWeaponFit(ShieldBattery));
		WeaponSlot->FitWeapon(ShieldBattery);
		Ship->AddWeapon(WeaponSlot);
	}
}

AETVShip * AETVGameModeBase::GetShipActor(int32 x, int32 y)
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

FVector AETVGameModeBase::GetPosition(int32 x, int32 y, int32 z)
{
	return FVector(-(TileSize / 2)*MapWidth + x*TileSize, -(TileSize / 2)*MapHeight + y*TileSize, z);
}

void AETVGameModeBase::EndTurn()
{
	// Close context menu if any is open
	for (AETVShip* CurShip : Ships)
	{
		// TODO Close context menu
		//CurShip->CloseMenu();
	}

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

		// Move control to AI
		// TODO Call into AI to do its thing
		// TODO AI calls NextTurn() when done
	}
}

void AETVGameModeBase::NextTurn()
{
	// Apply next turn
	CurrentTurn++;
	CurrentTurnTime = static_cast<float>(TurnTime);

	// Unpause
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	PlayerController->SetPause(false);
}

float AETVGameModeBase::GetCurrentTurnPercentage()
{
	return 1.0f - CurrentTurnTime / static_cast<float>(TurnTime);
}

void AETVGameModeBase::GetMouseOverTile(FETVTile& Tile)
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

	FVector WorldLocation;
	FVector WorldDirection;
	if (PlayerController->DeprojectMousePositionToWorld(WorldLocation, WorldDirection))
	{
		// Add height to tile map
		WorldDirection *= abs(TileHeight / WorldDirection.Z + WorldLocation.Z);
		WorldLocation += WorldDirection;

		// Check if on tile at all
		bool bInRangeX = UKismetMathLibrary::InRange_FloatFloat(WorldLocation.X, TileData[0].PointLeftTop.X, TileData.Last().PointRightBottom.X, false, false);
		bool bInRangeY = UKismetMathLibrary::InRange_FloatFloat(WorldLocation.Y, TileData[0].PointLeftTop.Y, TileData.Last().PointRightBottom.Y, false, false);
		if (bInRangeX && bInRangeY)
		{
			// Check each tile if in range
			for (auto &CurrentTileData : TileData)
			{
				bInRangeX = UKismetMathLibrary::InRange_FloatFloat(WorldLocation.X, CurrentTileData.PointLeftTop.X, CurrentTileData.PointRightBottom.X, false, false);
				bInRangeY = UKismetMathLibrary::InRange_FloatFloat(WorldLocation.Y, CurrentTileData.PointLeftTop.Y, CurrentTileData.PointRightBottom.Y, false, false);
				if (bInRangeX && bInRangeY)
				{
					//UE_LOG(LogTemp, Warning, TEXT("Loc (%s) -- Dir (%s)"), *WorldLocation.ToString(), *WorldDirection.ToString())
					Tile.Set(CurrentTileData.Tile);
					return;
				}
			}
		}
	}

	Tile.Invalidate();
}

void AETVGameModeBase::OnClickedMapTile()
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
			StopTargeting();
		}
	}

}

void AETVGameModeBase::OnReleasedMapTile()
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

void AETVGameModeBase::StartTargeting(AETVShip* TargetingInstigator, UETVActionTarget* Action)
{
	TargetingInstigator = TargetingInstigator;
	SelectedAction = Action;

	bTargeting = true;
}

void AETVGameModeBase::StopTargeting()
{
	bTargeting = false;

	// Set previous tile variables to invalid
	CurrentTile.Invalidate();
	LastTile.Invalidate();

	if (SelectedAction != nullptr)
	{
		// TODO
		// Get ship on last tile
		FPaperTileInfo TileInfoShip = TileMapComp->GetTile(LastTile.X, LastTile.Y, EETVTileLayer::Ship);

		//SelectedAction->SetTarget();
		SelectedAction->Perform();
	}
}
