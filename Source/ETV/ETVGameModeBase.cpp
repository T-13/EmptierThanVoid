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
}

// Called when the game starts or when spawned
void AETVGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	if (TileSet != nullptr)
	{
		MapGeneration();
	}
}

// Called every frame
void AETVGameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bTargeting)
	{
		int32 X;
		int32 Y;
		int32 Index;
		GetMouseOverTile(X, Y, Index);

		bool bIsDifferentTile = X != LastTileX || Y != LastTileY;
		// Only continue if hoevered tile is different than in previous frame and we are still in targeting
		if (bIsDifferentTile && bTargeting)
		{
			// Highlight currently hovered tile based on ability to perform an action
			FPaperTileInfo TileInfo;
			TileInfo.PackedTileIndex = Index;
			if (SelectedAction->CanPerform())
			{
				if (TileSetValidTarget != nullptr)
				{
					TileInfo.TileSet = TileSetValidTarget;
				}
			}
			else
			{
				if (TileSetInvalidTarget != nullptr)
				{
					TileInfo.TileSet = TileSetInvalidTarget;
				}
			}
			TileMapComp->SetTile(X, Y, 0, TileInfo);

			// Reset previous tile
			if (LastTileX >= 0 && LastTileY >= 0 && LastTileIndex >= 0)
			{
				FPaperTileInfo LastTileInfo;
				LastTileInfo.TileSet = TileSet;
				LastTileInfo.PackedTileIndex = LastTileIndex;
				TileMapComp->SetTile(LastTileX, LastTileY, 0, LastTileInfo);
			}

			// Set previous tile variables to currently hovered tile
			LastTileX = X;
			LastTileY = Y;
			LastTileIndex = Index;
		}
	}
}

void AETVGameModeBase::MapGeneration() {
	// Vector for spawn location (takes into account height, width and tile size)
	const FVector LocDim(-(MapHeight * TileSize / 2), -(MapWidth * TileSize / 2), 750);

	// Actor spawn parameters
	const FActorSpawnParameters SpawnInfo;

	// Rotate upwards to face the top-down camera
	const FRotator Rotator(0, 0, -90);

	// Spawning Tile Map Actor
	TileMapActor = GetWorld()->SpawnActor<APaperTileMapActor>(LocDim, Rotator, SpawnInfo);

	// Getting Tile Map Component from Tile Map Actor
	TileMapComp = TileMapActor->GetRenderComponent();

	// Create map and make it editable
	TileMapComp->CreateNewTileMap(MapWidth, MapHeight);
	TileMapComp->MakeTileMapEditable();

	// Paper tile info for SetTile function
	FPaperTileInfo TileInfo;

	// This is referenced in blueprint
	TileInfo.TileSet = TileSet;

	// For filling in Tiles
	for (int32 i = 0; i < MapHeight; i++) {
		for (int32 j = 0; j < MapWidth; j++) {

			// We have 2 sprites and they have to alternate between PackedTileIndex 0 and 1
			TileInfo.PackedTileIndex = (i + j) % 2;

			// Setting the tile with coordinates i and j
			TileMapComp->SetTile(i, j, 0, TileInfo);

			// Add to Tile Data
			FETVTileData CurrentTileData;
			CurrentTileData.X = j;
			CurrentTileData.Y = i;
			CurrentTileData.Index = TileInfo.PackedTileIndex;

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
	TileMapActor->OnClicked.AddDynamic(this, &AETVGameModeBase::OnClickMapTile);
}

void AETVGameModeBase::GetMouseOverTile(int32& X, int32& Y, int32& Index)
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
				X = CurrentTileData.X;
				Y = CurrentTileData.Y;
				Index = CurrentTileData.Index;

				break;
			}
		}
	}
}

void AETVGameModeBase::OnClickMapTile(AActor* Actor, FKey Key)
{
	if (bTargeting)
	{
		int32 X;
		int32 Y;
		int32 Index;
		GetMouseOverTile(X, Y, Index);

		StopTargeting();
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

	// Reset tile
	FPaperTileInfo LastTileInfo;
	LastTileInfo.TileSet = TileSet;
	LastTileInfo.PackedTileIndex = LastTileIndex;
	TileMapComp->SetTile(LastTileX, LastTileY, 0, LastTileInfo);

	// Set previous tile variables to invalid
	LastTileX = -1;
	LastTileY = -1;
	LastTileIndex = -1;

	// TODO Get ship on grid (layer 1)
	//SelectedAction->SetTarget();
	SelectedAction->Perform();
}
