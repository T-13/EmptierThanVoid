// Copyright (C) Team13. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Paper2DClasses.h"
#include "ConstructorHelpers.h"
#include "Kismet/KismetMathLibrary.h"
#include "ETVActionTarget.h"
#include "ETVShip.h"
#include "ETVShipCapital.h"
#include "ETVShipFighter.h"
#include "ETVShipRepairShip.h"
#include "ETVStructTile.h"
#include "ETVShipStatusUIWidget.h"
#include "ETVActionLogWidget.h"
#include "ETVActionLogEntryWidget.h"
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

	/* Generation */
	APaperTileMapActor* TileMapActor;
	UPaperTileMapComponent* TileMapComp;
	TArray<FETVTileData> TileData;

	AETVShip* Ship;
	AETVShipCapital* CapitalShip;
	AETVShipFighter* FighterShip;
	AETVShipRepairShip* RepairShip;
	TArray<AETVShip*> Ships;


	/* Game Loop */
	TArray<UETVAction*> MultiTurnActions;


	/* Targeting */
	bool bTargeting;
	FETVTile CurrentTile;
	FETVTile LastTile; // Reset this tile when mouse leaves it
	FETVTile PreDelayTile; // Reset this tile when click animation stops
	FETVTile ConfirmationTile; // Clicked once tile, waiting for another click (confirmation)

	UETVActionTarget* SelectedAction;

	// Ship Tiles
	UPaperTileSet* PlayerCapitalShip;
	UPaperTileSet* PlayerFighterShip;
	UPaperTileSet* EnemyCapitalShip;
	UPaperTileSet* EnemyFighterShip;

public:
	// Sets default values for this actor's properties
	AETVGameModeBase();

protected:
	// Called when the game starts or when spawned
	void BeginPlay() override;


	/* Generation */
	// Tile set of the board
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ETV Map")
	UPaperTileSet* TileSet;

	// Higlighted tile set of the board for valid (index 0) and invalid (index 1) targets
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ETV Map")
	UPaperTileSet* TileSetTarget;

	// Higlighted tile set of the board for valid (index 0) and invalid (index 1) targets on click
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ETV Map")
	UPaperTileSet* TileSetTargetClick;

	// The widget class for the ContextMenu for Ship class
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ETV Ship", meta = (BlueprintProtected = "true"))
	TSubclassOf<UETVShipContextMenuWidget> ContextMenu;

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


	/* Game Loop */
	// Elapsed game time since start of first turn, in seconds
	UPROPERTY(BlueprintReadOnly, Category = "ETV Game")
	float ElapsedTime;

	// Time each turn lasts, in seconds
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ETV Game")
	int32 TurnTime;

	// Current turn number
	UPROPERTY(BlueprintReadOnly, Category = "ETV Game")
	int32 CurrentTurn;

	// Time left until end of turn, in seconds
	UPROPERTY(BlueprintReadOnly, Category = "ETV Game")
	float CurrentTurnTime;

	// Disable AI (debug)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ETV Game")
	bool bDisableAI;


	/* Targeting */
	// Start with targeting enabled (debug)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ETV Map")
	bool bTargetingOnStart;

	// The widget class for the ShipStatusUI
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ETV UI", meta = (BlueprintProtected = "true"))
	TSubclassOf<UETVShipStatusUIWidget> ShipStatusUIClass;

	// The pointer to accses the ShipStatusUI Widget
	UPROPERTY()
	UETVShipStatusUIWidget* ShipStatusUI;

	// For ActionLog UI
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ETV ActionLog")
	TSubclassOf<class UETVActionLogWidget> ActionLogWidget;

	UPROPERTY()
	class UETVActionLogWidget* ActionLogClass;

	// Reference to THE ONE ship with the opened context menu
	UPROPERTY()
	class AETVShip* LastClickedShip;
	
public:
	// Called every frame
	void Tick(float DeltaTime) override;


	/* Generation */
	// Generate map of size MapWidth and MapHeight
	UFUNCTION()
	void MapGeneration();

	// Fires when map finishes generating
	UFUNCTION(BlueprintImplementableEvent, Category = "ETV Map")
	void MapGenerated(APaperTileMapActor* TileMapActor);

	// Generate Player and Enemy Ships on each side of the map
	UFUNCTION(BlueprintCallable, Category = "ETV Map")
	void GenerateShips();

	// Spawn ShipActor on the correct X and Y
	UFUNCTION()
	void SpawnShip(int32 x, int32 y, UPaperTileSet* type);

	// Spawn WeaponActor for Ship
	UFUNCTION()
	void SpawnWeapon(int32 NewX, int32 NewY, AETVShip* Ship, int32 type, int32 level);

	// Spawn Actions for Ship and Weapons
	UFUNCTION()
	void SpawnActions(AETVShip* Ship);

	// Get Ships Actor from passing in Tiles x and y
	UFUNCTION()
	AETVShip* GetShipActor(int32 x, int32 y);

	// Get Ship's world location from tile
	UFUNCTION()
	FVector GetPosition(int32 x, int32 y, float z = 0.1f);

	// Set Ship's tile location
	UFUNCTION()
	void SetPosition(int32 ToX, int32 ToY, int32 FromX, int32 FromY);


	/* Game Loop */
	// End current turn
	UFUNCTION(BlueprintCallable, Category = "ETV Game")
	void EndTurn();

	// Start next turn
	UFUNCTION(BlueprintCallable, Category = "ETV Game")
	void NextTurn();

	// Get current turn time percentage until end of turn
	UFUNCTION(BlueprintCallable, Category = "ETV Game")
	float GetCurrentTurnPercentage();

	// Add multi-turn action for execution in subsequent turns automatically
	UFUNCTION(BlueprintCallable, Category = "ETV Game")
	void AddMultiTurnAction(UETVAction* Action);

	// Remove multi-turn action to stop execution in subsequent turns automatically
	UFUNCTION(BlueprintCallable, Category = "ETV Game")
	void RemoveMultiTurnAction(UETVAction* Action);


	/* Targeting */
	// Get targeting enabled
	UFUNCTION(BlueprintCallable, Category = "ETV Targeting")
	bool IsTargeting();

	// Return if mouse is over the tile map
	UFUNCTION(BlueprintCallable, Category = "ETV Targeting")
	bool IsPositionOnTileMap(const FVector Location);

	// Get tile position below mouse pointer
	UFUNCTION(BlueprintCallable, Category = "ETV Targeting")
	void GetMouseOverTile(/*out*/ FETVTile& Tile);

	// Target selection effect (Tile Maps don't support animations at this time)
	UFUNCTION()
	void OnClickedSelectMapTile();

	// Target deselection effect (Tile Maps don't support animations at this time)
	UFUNCTION()
	void OnReleasedSelectMapTile();

	// Targeting cancelation
	UFUNCTION()
	void OnClickedCancelTargeting();

	// Starts targeting, handles ETV Action calls and stops targeting after target is selected
	UFUNCTION(BlueprintCallable, Category = "ETV Targeting")
	void StartTargeting(UETVActionTarget* Action);

	// Stops targeting, resetting targeting mode
	UFUNCTION(BlueprintCallable, Category = "ETV Targeting")
	void StopTargeting(bool bSuccess = true);


	/* Player Action Handling */
	// Update last clicked ship
	UFUNCTION(BlueprintCallable, Category = "ETV Click")
	void ShipClicked(AETVShip* ClickedShip);

	// Has a ship opened context menu
	UFUNCTION(BlueprintCallable, Category = "ETV Click")
	bool WasShipClickedRecently();

	UFUNCTION()
	AETVShip* GetLastClickedShip();


	/* Get Widgets */
	// Get log widget
	UFUNCTION(BlueprintCallable, Category = "ETV UI")
	UETVActionLogWidget* GetLogWidget();

	// Get Ship List widget
	UFUNCTION(BlueprintCallable, Category = "ETV UI")
	UETVShipStatusUIWidget* GetShipListWidget();
};
