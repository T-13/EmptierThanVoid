// Copyright (C) Team13. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Paper2DClasses.h"
#include "ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "ETVActionTarget.h"
#include "ETVShip.h"
#include "GameFramework/GameModeBase.h"
#include "ETVGameModeBase.generated.h"

USTRUCT(BlueprintType, DisplayName = "ETV Tile Data")
struct FETVTileData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile Data")
	FVector2D PointLeftTop;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile Data")
	FVector2D PointRightBottom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile Data")
	int32 X;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile Data")
	int32 Y;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile Data")
	int32 Index;
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
	
	// Targeting
	bool bTargeting;
	UETVActionTarget* SelectedAction;
	AETVShip* TargetingInstigator;
	TArray<FETVTileData> TileData;
	int32 LastTileX;
	int32 LastTileY;
	int32 LastTileIndex;

public:
	// Sets default values for this actor's properties
	AETVGameModeBase();

protected:
	// Called when the game starts or when spawned
	void BeginPlay() override;

	// Tile set of the board
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ETV Map")
	UPaperTileSet* TileSet;

	// Higlighted tile set of the board for valid target
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ETV Map")
	UPaperTileSet* TileSetValidTarget;

	// Higlighted tile set of the board for invalid target
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ETV Map")
	UPaperTileSet* TileSetInvalidTarget;

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
	// Called every frame
	void Tick(float DeltaTime) override;

	// Generate map of size MapWidth and MapHeight
	UFUNCTION()
	void MapGeneration();

	UFUNCTION(BlueprintImplementableEvent, Category = "ETV Map")
	void MapGenerated(APaperTileMapActor* TileMapActor);

	UFUNCTION(BlueprintCallable, Category = "ETV Map")
	void GetMouseOverTile(/*out*/ int32& X, /*out*/ int32& Y, /*out*/ int32& Index);

	UFUNCTION()
	void OnClickMapTile(AActor* Actor, FKey Key);


	// Starts targeting, handles ETV Action calls and stops targeting after target is selected
	UFUNCTION(BlueprintCallable, Category = "ETV Targeting")
	void StartTargeting(UPARAM(ref) AETVShip* ActionInstigator, UPARAM(ref) UETVActionTarget* Action);

	// Stops targeting, resetting targeting mode
	UFUNCTION(BlueprintCallable, Category = "ETV Targeting")
	void StopTargeting();
};
