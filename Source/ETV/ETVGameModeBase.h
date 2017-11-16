// Copyright (C) Team13. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Paper2DClasses.h"
#include "ConstructorHelpers.h"
#include "GameFramework/GameModeBase.h"
#include "ETVGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class ETV_API AETVGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AETVGameModeBase();

protected:
	// Called when the game starts or when spawned
	void BeginPlay() override;

	// Tile set of the board
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ETV Map")
	UPaperTileSet* TileSet;

	// Size of one tile
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ETV Map")
	int32 TileSize;

	// Width of the board
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ETV Map")
	float MapWidth;

	// Height of the board
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ETV Map")
	float MapHeight;
	
public:
	// Generate map of size MapWidth and MapHeight
	UFUNCTION()
	void MapGeneration();
	
};
