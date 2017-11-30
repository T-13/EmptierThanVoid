// Copyright (C) Team13. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Paper2DClasses.h"
#include "ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "ETVActionTarget.h"
#include "ETVShip.h"
#include "ETVShipCapital.h"
#include "ETVShipFighter.h"
#include "ETVStructTile.h"
#include "EngineUtils.h"
#include "GameFramework/GameModeBase.h"
#include "ETVGameModeBase.generated.h"

UENUM(BlueprintType)
enum EETVTileLayer
{
	Effect	UMETA(DisplayName = "Effects"),
	Ship	UMETA(DisplayName = "Ships"),
	Target	UMETA(DisplayName = "Target"),
	Board	UMETA(DisplayName = "Board")
};

UENUM(BlueprintType)
enum EETVTargetValidity
{
	Valid	UMETA(DisplayName = "Valid"),
	Invalid	UMETA(DisplayName = "Invalid")
};

/**
 * Base game mode class.
 */
UCLASS()
class ETV_API AETVGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

	APaperTileMapActor* TileMapActor;
	UPaperTileMapComponent* TileMapComp;
	TArray<FETVTileData> TileData;

	AETVShip* Ship;
	TArray<AETVShip*> Ships;
	
	// Targeting
	bool bTargeting;
	FETVTile CurrentTile;
	FETVTile LastTile; // Reset this tile when mouse leaves it
	FETVTile PreDelayTile; // Reset this tile when click animation stops

	UETVActionTarget* SelectedAction;
	AETVShip* TargetingInstigator;

public:
	// Sets default values for this actor's properties
	AETVGameModeBase();

protected:
	// Called when the game starts or when spawned
	void BeginPlay() override;

	// Tile set of the board
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ETV Map")
	UPaperTileSet* TileSet;

	// Higlighted tile set of the board for valid (index 0) and invalid (index 1) targets
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ETV Map")
	UPaperTileSet* TileSetTarget;

	// Higlighted tile set of the board for valid (index 0) and invalid (index 1) targets on click
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ETV Map")
	UPaperTileSet* TileSetTargetClick;

	// Player capital ship
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ETV Map")
	UPaperTileSet* PlayerCapitalShip;

	// Player fighter ship
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ETV Map")
	UPaperTileSet* PlayerFighterShip;

	// Enemy capital ship
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ETV Map")
	UPaperTileSet* EnemyCapitalShip;

	// Enemy fighter ship
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ETV Map")
	UPaperTileSet* EnemyFighterShip;

	// The widget class for the ContextMenu for Ship class
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ETV Ship", meta = (BlueprintProtected = "true"))
	TSubclassOf<class UETVShipContextMenuWidget> ContextMenu;

	// Sprite for Ship Actor (Should be transparent)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ETV Ship")
	UPaperSprite* Sprite;

	// Size of one tile
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ETV Map")
	int32 TileSize;

	// Width of the board
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ETV Map")
	float MapWidth;

	// Height of the board
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ETV Map")
	float MapHeight;

	// Start with targeting enabled (debug)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ETV Map")
	bool bTargetingOnStart;
	
public:
	// Called every frame
	void Tick(float DeltaTime) override;

	// Generate map of size MapWidth and MapHeight
	UFUNCTION()
	void MapGeneration();

	// Fires when map finishes generating
	UFUNCTION(BlueprintImplementableEvent, Category = "ETV Map")
	void MapGenerated(APaperTileMapActor* TileMapActor);

	// Get tile position below mouse pointer
	UFUNCTION(BlueprintCallable, Category = "ETV Map")
	void GetMouseOverTile(/*out*/ FETVTile& Tile);

	// Target selection effect (Tile Maps don't support animations at this time)
	UFUNCTION()
	void OnClickedMapTile(AActor* Actor, FKey Key);

	// Target deselection effect (Tile Maps don't support animations at this time)
	UFUNCTION()
	void OnReleasedMapTile(AActor* Actor, FKey Key);

	// Starts targeting, handles ETV Action calls and stops targeting after target is selected
	UFUNCTION(BlueprintCallable, Category = "ETV Targeting")
	void StartTargeting(UPARAM(ref) AETVShip* ActionInstigator, UPARAM(ref) UETVActionTarget* Action);

	// Stops targeting, resetting targeting mode
	UFUNCTION(BlueprintCallable, Category = "ETV Targeting")
	void StopTargeting();

	// Generate Player and Enemy Ships on each side of the map
	UFUNCTION(BlueprintCallable, Category = "ETV Targeting")
	void GenerateShips();

	// Spawn ShipActor on the correct X and Y
	UFUNCTION()
	void SpawnShip(int32 x, int32 y, UPaperTileSet* type);

	// Get Ships Actor from passing in Tiles x and y
	UFUNCTION()
	AETVShip* GetShipActor(int32 x, int32 y);

	// Get Ships location from Tiles x and y;
	UFUNCTION()
	FVector GetPosition(int32 x, int32 y, int32 z = -449);
};
