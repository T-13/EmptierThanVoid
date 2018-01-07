// Copyright (C) Team13. All rights reserved.

#pragma once

#include "ETVWeaponSlot.h"
#include "CoreMinimal.h"
#include "PaperSpriteActor.h"
#include "ETVShipContextMenuWidget.h"
#include "ETVShip.generated.h"

UENUM(BlueprintType)
enum class EETVShipType : uint8
{
	PlayerShip	UMETA(DisplayName = "Player"),
	EnemyShip	UMETA(DisplayName = "Enemy")
};

UENUM(BlueprintType)
enum class EETVShipClass : uint8
{
	Capital	UMETA(DisplayName = "Capital"),
	Fighter	UMETA(DisplayName = "Fighter"),
	Repair UMETA(DisplayName = "Repair")
};



/**
* Abstract base Ship class.
*/
UCLASS(Abstract)
class ETV_API AETVShip : public APaperSpriteActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AETVShip();
	void Init(FString NewName, int32 MaxHP, int32 ShieldP, int32 NewShieldRechargeTime, int32 NewMoveRange, int32 Speed);

	void InitRandom(FString NewName);

	void InitRandomWithLevel(FString NewName, int32 PowerLvl);


protected:
	// Called when the game starts or when spawned
	void BeginPlay() override;


	// Name of the ship
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ETV Ship")
	FString Name;

	// Health points of the ship
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ETV Ship", meta = (ClampMin = "0.0"))
	int32 HealthPoints;

	// Maximum health points of the ship
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ETV Ship", meta = (ClampMin = "0.0"))
	int32 MaximumHealthPoints;
	
	// Bonus to health for ships
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ETV Ship", meta = (ClampMin = "0.0"))
	int32 ShieldPoints;

	// Time for the shields to recharge
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ETV Ship", meta = (ClampMin = "0.0"))
	int32 ShieldRechargeTime;

	// Weapon slots
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ETV Ship")
	TArray<UETVWeaponSlot*> Weapons;

	UETVWeaponSlot* WeaponSlot;

	// Integers (x,y) that represent a point on a sprite where the attachement goes
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ETV Ship")
	TArray<int32> AttachementSlots;

	// Area that ship is allowed to move to
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ETV Ship", meta = (ClampMin = "1.0"))
	int32 MoveRange;

	// Integers (x,y) for possible fields that ship can move to
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ETV Ship")
	TArray<int32> Fields;

	// Actions available on ship
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ETV Ship")
	TArray<UETVAction*> Actions;

	// How fast ship moves
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ETV Ship", meta = (ClampMin = "1.0"))
	int32 ShipSpeed;

	// The widget class for the ContextMenu
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ETV Ship", meta = (BlueprintProtected = "true"))
	TSubclassOf<UETVShipContextMenuWidget> ContextMenuClass;

	// Instance of our ContextMenu
	UPROPERTY()
	class UETVShipContextMenuWidget* CurrentContextMenu;

	// Tells us if ContextMenu for this ship is Open
	UPROPERTY()
	bool IsContextMenuOpen;

	// Type of Ship (0 = player ship, 1 = enemy ship)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ETV Ship")
	EETVShipType Type;

	// Ship class
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ETV Ship")
	EETVShipClass Class;

	// Current X and Y position
	UPROPERTY()
	int32 X;

	UPROPERTY()
	int32 Y;

	UPROPERTY()
	FVector PositionInWorld;

	UPROPERTY()
	int32 Level;

public:
	// Called every frame
	void Tick(float DeltaTime) override;

	UFUNCTION()
	void SetCurrentPosition(int32 NewX, int32 NewY);

	UFUNCTION()
	void AddWeapon(UETVWeaponSlot* Weapon);

	UFUNCTION()
	void AddAction(UETVAction* Action);

	UFUNCTION()
	TArray<UETVWeaponSlot*> GetWeapons();

	UFUNCTION()
	virtual void RechargeShields();

	UFUNCTION()
	virtual void GetReport();

	UFUNCTION()
	void SpawnContextMenu(AActor *Actor, FKey Key);

	UFUNCTION()
	int32 GetHP();

	UFUNCTION()
	void SetHP(int32 newValue);

	UFUNCTION()
	int32 GetShields();

	UFUNCTION()
	void SetShields(int32 newValue);

	UFUNCTION()
	bool CanMove();

	UFUNCTION()
	void MoveToTile(int32 NewX, int32 NewY);

	UFUNCTION(BlueprintCallable)
	bool IsEnemy();

	UFUNCTION(BlueprintCallable)
	int32 GetX() const { return X; }

	UFUNCTION(BlueprintCallable)
	int32 GetY() const { return Y; }

	UFUNCTION()
	void CloseContextMenu();

	UFUNCTION()
	void UnconditionallyCloseContextMenu();

	// Returns a multiplier for the effectiveness of ship's actions depending on its status
	UFUNCTION()
	virtual float GetMultiplier();

	UFUNCTION(BlueprintCallable)
	void ClosingContextMenu();

	UFUNCTION(BlueprintCallable)
	void SetContextMenu(TSubclassOf<class UETVShipContextMenuWidget> ContextMenu);

	// Set Ship Type to Enemy
	UFUNCTION()
	void SetTypeToEnemy();

	// Just for checking in LOG
	UFUNCTION()
	FString GetShipType();

	// Just for checking in LOG
	UFUNCTION()
	FString GetShipClass();

	UFUNCTION()
	FString GetShipName();

	UFUNCTION()
	int32 GetLevel() const { return Level; }

	UFUNCTION()
	EETVShipClass GetClass() const { return Class; }

	UFUNCTION()
	void SpawnWeapons();

	UFUNCTION()
	int32 GetMaxHP() const { return MaximumHealthPoints; }
};
