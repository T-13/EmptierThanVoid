// Copyright (C) Team13. All rights reserved.

#include "ETVGameModeBase.h"

// Sets default values
AETVGameModeBase::AETVGameModeBase()
{
	// Set this actor to call Tick() every frame
	PrimaryActorTick.bCanEverTick = true;

	TileSize = 32;
	MapWidth = 25.0f;
	MapHeight = 25.0f;

	// Targeting
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
	}
}

// Called every frame
void AETVGameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
	const FVector LocDim(-(MapHeight * TileSize / 2), -(MapWidth * TileSize / 2), -450);

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
	TileMapActor->OnClicked.AddDynamic(this, &AETVGameModeBase::OnClickedMapTile);
	TileMapActor->OnReleased.AddDynamic(this, &AETVGameModeBase::OnReleasedMapTile);
}

void AETVGameModeBase::GetMouseOverTile(FETVTile& Tile)
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery1);
	FHitResult Hit;

	if (PlayerController->GetHitResultUnderCursorForObjects(ObjectTypes, true, Hit))
	{
		for (auto &CurrentTileData : TileData)
		{
			bool bInRangeX = UKismetMathLibrary::InRange_FloatFloat(Hit.Location.X, CurrentTileData.PointLeftTop.X, CurrentTileData.PointRightBottom.X, false, false);
			bool bInRangeY = UKismetMathLibrary::InRange_FloatFloat(Hit.Location.Y, CurrentTileData.PointLeftTop.Y, CurrentTileData.PointRightBottom.Y, false, false);
			if (bInRangeX && bInRangeY)
			{
				Tile.Set(CurrentTileData.Tile);
				return;
			}
		}
	}

	Tile.Invalidate();
}

void AETVGameModeBase::OnClickedMapTile(AActor* Actor, FKey Key)
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

void AETVGameModeBase::OnReleasedMapTile(AActor* Actor, FKey Key)
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

	// Spawning Fighter Sihps on each side
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
	if (type == PlayerCapitalShip || type == EnemyCapitalShip)
		Ship = GetWorld()->SpawnActor<AETVShipCapital>(LocDim, Rotator, SpawnInfo);
	else if (type == PlayerFighterShip || type == EnemyFighterShip)
		Ship = GetWorld()->SpawnActor<AETVShipFighter>(LocDim, Rotator, SpawnInfo);

	Ship->SetContextMenu(ContextMenu);
	Ship->GetRenderComponent()->SetMobility(EComponentMobility::Movable);
	Ship->GetRenderComponent()->SetSprite(Sprite);

	// Setting sprite color to transparent
	Ship->GetRenderComponent()->SetSpriteColor(FLinearColor(1.0f, 1.0f, 1.0f, 0.0f));

	Ships.Add(Ship);
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

