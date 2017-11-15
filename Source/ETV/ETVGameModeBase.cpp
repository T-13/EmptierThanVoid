// Copyright (C) Team13. All rights reserved.

#include "ETVGameModeBase.h"

// Sets default values
AETVGameModeBase::AETVGameModeBase()
{
	TileSize = 32;
	MapWidth = 25.0f;
	MapHeight = 25.0f;
}


// Called when the game starts or when spawned
void AETVGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	MapGeneration();
}


void AETVGameModeBase::MapGeneration() {
	// Vector for locations (x,y=0,z)
	FVector LocVector;
	LocVector.Y = 0;

	// Vector for tile dimensions (x,y=0,z)
	FVector DimVector(TileSize, 0, TileSize);

	// Setting x coordinate for vector
	LocVector.X = MapHeight;

	// Setting y coordinate for vector
	LocVector.Z = MapWidth;

	// Vector for Location * Dimension (x, y)
	//FVector LocDim(LocVector.X * DimVector.X, 0, LocVector.Z * DimVector.Z);
	FVector LocDim(-270, 725, 750);

	// Actor spawn parameters
	FActorSpawnParameters SpawnInfo;

	// For rotation on X axis
	FRotator Rotator(0, 0, 90);

	// Spawning Tile Map Actor
	APaperTileMapActor* TileMapActor = GetWorld()->SpawnActor<APaperTileMapActor>(LocDim, Rotator, SpawnInfo);

	// Getting Tile Map Component from Tile Map Actor
	UPaperTileMapComponent* TimeMapComp = TileMapActor->GetRenderComponent();

	// Create map and make it editable
	TimeMapComp->CreateNewTileMap(MapWidth, MapHeight);
	TimeMapComp->MakeTileMapEditable();

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
			TimeMapComp->SetTile(i, j, 0, TileInfo);
		}
	}
}
