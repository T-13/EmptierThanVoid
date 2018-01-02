// Copyright (C) Team13. All rights reserved.

#include "ETVGameModeBase.h"
#include "ETVWeaponLaser.h"
#include "ETVWeaponTorpedo.h"
#include "ETVWeaponRepairArm.h"
#include "ETVWeaponShieldBattery.h"
#include "ETVActionTarget_Fire.h"
#include "ETVActionTarget_Move.h"
#include "ETVCameraDirector.h"
#include "WidgetLayoutLibrary.h"
#include "UserWidget.h"
#include "ETVCalculator.h"
//#include "DrawDebugHelpers.h" // Uncomment for debug drawing

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

	// Spawn Widget for Action Log
	ActionLogClass = CreateWidget<UETVActionLogWidget>(GetWorld(), ActionLogWidget);
	ActionLogClass->SetPositionInViewport(UKismetMathLibrary::MakeVector2D(0, 0));
	ActionLogClass->SetDesiredSizeInViewport(FVector2D(UWidgetLayoutLibrary::GetViewportSize(GetWorld())));
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

		FVector2D temp = UWidgetLayoutLibrary::GetViewportSize(GetWorld());
		
		// Define the size of the element
		int32 WidthOfShipStatusUI = temp.X*0.5;
		int32 HeightOfShipStatusUI = 180;

		// Set the location to bottom left corner
		temp.X = temp.X*0.02;
		temp.Y = temp.Y*0.995 - HeightOfShipStatusUI;
		ShipStatusUI->SetPositionInViewport(temp);

		// Resize to correct size
		ShipStatusUI->SetDesiredSizeInViewport(UKismetMathLibrary::MakeVector2D(WidthOfShipStatusUI, HeightOfShipStatusUI));

		ShipStatusUI->AddToViewport();

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
	FVector LocDim = GetPosition(x, y);
	LocDim.Z = 0.1f;

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

		for (int32 i = 0; i < 4; i++) {
			if (i == 0)  // Laser
			{
				SpawnWeapon(x, y, CapitalShip, AETVWeapon::DamageHull, 200);
			}
			else if (i == 1) // Torpedo
			{
				SpawnWeapon(x, y, CapitalShip, AETVWeapon::DamageShieldThenHull, 200);
			}
			else if (i == 2) // RepairArm
			{
				SpawnWeapon(x, y, CapitalShip, AETVWeapon::HealHull, 200);
			}
			else if (i == 3) // ShieldBattery
			{
				SpawnWeapon(x, y, CapitalShip, AETVWeapon::HealShield, 200);
			}
		}

		if (type == EnemyCapitalShip)
			CapitalShip->SetTypeToEnemy();
		else
			SpawnActions(CapitalShip);

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

		for (int32 i = 0; i < 3; i++) {
			if (i == 0)  // Laser
			{
				SpawnWeapon(x, y, FighterShip, AETVWeapon::DamageHull, 100);
			}
			else if (i == 1) // Torpedo
			{
				SpawnWeapon(x, y, FighterShip, AETVWeapon::DamageShieldThenHull, 100);
			}
			else if (i == 2) // ShieldBattery
			{
				SpawnWeapon(x, y, FighterShip, AETVWeapon::HealShield, 100);
			}
		}

		if (type == EnemyFighterShip)
			FighterShip->SetTypeToEnemy();
		else
			SpawnActions(FighterShip);

		Ships.Add(FighterShip);
	}
}

void AETVGameModeBase::SpawnWeapon(int32 NewX, int32 NewY, AETVShip* Ship, int32 type, int32 level)
{
	UETVWeaponSlot* WeaponSlot = NewObject<UETVWeaponSlot>();
	WeaponSlot->Init(level, level);

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
			Laser->InitRandom("Laser", level);
		} while (!WeaponSlot->DoesWeaponFit(Laser));
		WeaponSlot->FitWeapon(Laser);
		Ship->AddWeapon(WeaponSlot);


	}
	else if (type == AETVWeapon::DamageHull)
	{
		AETVWeaponTorpedo* Torpedo;
		Torpedo = GetWorld()->SpawnActor<AETVWeaponTorpedo>(LocDim, Rotator, SpawnInfo);

		do {
			Torpedo->InitRandom("Torpedo", level);
		} while (!WeaponSlot->DoesWeaponFit(Torpedo));
		WeaponSlot->FitWeapon(Torpedo);
		Ship->AddWeapon(WeaponSlot);
	}
	else if (type == AETVWeapon::HealHull)
	{
		AETVWeaponRepairArm* RepairArm;
		RepairArm = GetWorld()->SpawnActor<AETVWeaponRepairArm>(LocDim, Rotator, SpawnInfo);

		do {
			RepairArm->InitRandom("RepairArm", level);
		} while (!WeaponSlot->DoesWeaponFit(RepairArm));
		WeaponSlot->FitWeapon(RepairArm);
		Ship->AddWeapon(WeaponSlot);
	}
	else if (type == AETVWeapon::HealShield)
	{
		AETVWeaponShieldBattery* ShieldBattery;
		ShieldBattery = GetWorld()->SpawnActor<AETVWeaponShieldBattery>(LocDim, Rotator, SpawnInfo);

		do {
			ShieldBattery->InitRandom("ShieldBattery", level);
		} while (!WeaponSlot->DoesWeaponFit(ShieldBattery));
		WeaponSlot->FitWeapon(ShieldBattery);
		Ship->AddWeapon(WeaponSlot);
	}
}

void AETVGameModeBase::SpawnActions(AETVShip* Ship)
{
	for (UETVWeaponSlot* w : Ship->GetWeapons())
	{
		UETVActionTarget_Fire *Fire = NewObject<UETVActionTarget_Fire>();
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
			UE_LOG(LogTemp, Error, TEXT("GetMouseOverTile(): Pawn not set to AETVCameraDirector!"))
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
					UE_LOG(LogTemp, Warning, TEXT("Loc (%s) -- Dir (%s) -- Zoom %g"), *WorldLocation.ToString(), *WorldDirection.ToString(), Camera->GetZoom())
					*/

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

void AETVGameModeBase::StartTargeting(UETVActionTarget* Action)
{
	SelectedAction = Action;

	bTargeting = true;
}

void AETVGameModeBase::StopTargeting()
{
	bTargeting = false;

	// Set previous tile variables to invalid
	LastTile.Invalidate();
	CurrentTile.Invalidate();

	// Make sure nothing stopped targeting
	if (SelectedAction != nullptr)
	{
		// Double check that if we can perform, then perform
		if (SelectedAction->CanPerform())
		{
			SelectedAction->Perform();
		}
	}
}

void AETVGameModeBase::ShipClicked(AETVShip * ClickedShip)
{
	LastClickedShip = ClickedShip;
}

bool AETVGameModeBase::WasShipClickedRecently()
{
	return (LastClickedShip != nullptr);
}

AETVShip * const AETVGameModeBase::GetLastClickedShip()
{
	return LastClickedShip;
}

UETVActionLogWidget* AETVGameModeBase::GetLogWidget() {
	return ActionLogClass;
}

UETVShipStatusUIWidget * AETVGameModeBase::GetShipListWidget()
{
	return ShipStatusUI;
}
