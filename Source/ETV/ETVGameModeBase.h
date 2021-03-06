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
	Target	UMETA(DisplayName = "Target"),
	Effect	UMETA(DisplayName = "Effects"),
	Ship	UMETA(DisplayName = "Ships"),
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
	EETVShipType CurrentTurnSide;
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
	UPaperTileSet* PlayerRepairShip;
	UPaperTileSet* EnemyRepairShip;

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

	// Hidden tile set of the board out of visibility effect
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ETV Map")
	UPaperTileSet* TileSetHidden;

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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ETV Targeting")
	bool bTargetingOnStart;


	/* Visibility */
	// Makes all tiles visible (debug)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ETV Visibility")
	bool bIgnoreVisibility;

	// Shows visibility for enemy during its turn (debug)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ETV Visibility")
	bool bShowEnemyVisibility;


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
	void SpawnShipType(int32 x, int32 y, UPaperTileSet* type, FString name);

	// Loop for spawning many Ships of the same type
	void SpawnShipsLoop(UPaperTileSet* Player, UPaperTileSet* Enemy, int32 NumOfShips, TArray<FString> ShipNames);

	template<class T>
	void SpawnShip(int32 x, int32 y, FString name, bool Enemy);

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

	// Destroy Ship Actor and reset Tile
	UFUNCTION()
	void DestroyShip(AETVShip* DestroyedShip);

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

	// Returns if current turn is AI controlled
	UFUNCTION(BlueprintCallable, Category = "ETV Game")
	bool IsCurrentTurnAI() const { return !bDisableAI && CurrentTurnSide == EETVShipType::EnemyShip; }

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


	/* Visibility */
	// Returns distance between 2 tiles in amount of tiles
	UFUNCTION(BlueprintCallable, Category = "ETV Visibility")
	float GetTiledDistance(FVector2D TileA, FVector2D TileB);

	// Updates board visibility effects if player side (or debug) and optionally returns visible tile positions
	UFUNCTION(BlueprintCallable, Category = "ETV Visibility")
	void UpdateVisibleTiles(EETVShipType Side, /*out*/ TArray<FVector2D>& VisibleTiles);
	void UpdateVisibleTiles(EETVShipType Side);

	// Returns all visible ships for the given side (filters Ships from AETVGameModeBase)
	UFUNCTION(BlueprintCallable, Category = "ETV Visibility")
	void GetVisibleShips(EETVShipType Side, /*out*/ TArray<AETVShip*>& VisibleShips);

	// Returns visibility of the given tile (if visible by any ship on given side)
	UFUNCTION(BlueprintCallable, Category = "ETV Visibility")
	bool IsTileVisible(FVector2D Tile, EETVShipType Side = EETVShipType::PlayerShip);

	// Set tile visibility (fog effect)
	UFUNCTION(BlueprintCallable, Category = "ETV Visibility")
	void SetTileVisibilityEffect(int32 X, int32 Y, bool bVisible);


	/* Player Action Handling */
	// Update last clicked ship
	UFUNCTION(BlueprintCallable, Category = "ETV Click")
	void ShipClicked(AETVShip* ClickedShip);

	// Has a ship opened context menu
	UFUNCTION(BlueprintCallable, Category = "ETV Click")
	bool WasShipClickedRecently();

	UFUNCTION()
	AETVShip* GetLastClickedShip() const { return LastClickedShip; }


	/* Widget and Ship Interaction */
	// Get log widget
	UFUNCTION(BlueprintCallable, Category = "ETV UI")
	UETVActionLogWidget* GetLogWidget() const { return ActionLogClass; }

	// Get Ship List widget
	UFUNCTION(BlueprintCallable, Category = "ETV UI")
	UETVShipStatusUIWidget* GetShipListWidget() const { return ShipStatusUI; }

	UFUNCTION()
	bool IsShipNameUsed(FString Name);

	UFUNCTION()
	bool TileHasShip(int32 x, int32 y);

	UFUNCTION()
	UPaperTileSet* GetShipSprite(AETVShip* Ship);

	UFUNCTION()
	APaperTileMapActor* GetTileMapActor() const { return TileMapActor; }
};


template<class T>
FORCEINLINE void AETVGameModeBase::SpawnShip(int32 x, int32 y, FString name, bool Enemy)
{
	FVector LocDim = GetPosition(x, y);
	LocDim.Z = 0.1f;

	// Actor spawn parameters
	const FActorSpawnParameters SpawnInfo;

	// Rotate upwards to face the top-down camera
	const FRotator Rotator(0, 0, -90);

	// Spawn and Initialize Ship
	T* Ship;
	Ship = GetWorld()->SpawnActor<T>(LocDim, Rotator, SpawnInfo);
	Ship->InitRandom(name);
	Ship->SetCurrentPosition(x, y);
	Ship->SetContextMenu(ContextMenu);
	Ship->GetRenderComponent()->SetMobility(EComponentMobility::Movable);
	Ship->GetRenderComponent()->SetSprite(Sprite);

	// Setting sprite color to transparent
	Ship->GetRenderComponent()->SetSpriteColor(FLinearColor(1.0f, 1.0f, 1.0f, 0.0f));

	if (Enemy)
		Ship->SetTypeToEnemy();

	SpawnActions(Ship);

	Ships.Add(Ship);
}
