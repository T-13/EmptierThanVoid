// Copyright (C) Team13. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ETVAction.generated.h"

UENUM(BlueprintType)
enum class EETVActionAvailability : uint8
{
	ActionUnavailable	UMETA(DisplayName = "Unavailable"),
	ActionAvailable		UMETA(DisplayName = "Available")
};

/**
 * Abstract base Action class.
 * Actions are attached to assets (eg. ships, weapons).
 */
UCLASS(Abstract)
class ETV_API UETVAction : public UObject
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	UETVAction();

	AETVShip* OwnerShip;
	AETVWeapon* OwnerWeapon;

protected:
	// Display name of the action
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ETV Action")
	FName Name;

	// Minimal range of the action
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ETV Action", meta = (ClampMin = "0.0"))
	int32 MinRange;

	// Maximal range of the action
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ETV Action", meta = (ClampMin = "100.0"))
	int32 MaxRange;

	// [NOT YET IMPLEMENTED] Availability of the action (0 = not shown in UI, 1 = shown in UI)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ETV Action", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	EETVActionAvailability Available;

	// Does action end the turn immediately after use
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ETV Action")
	bool bEndsTurn;

public:
	UFUNCTION()
	virtual void Init(AETVShip* OwnerShipPtr, AETVWeapon* OwnerWeaponPtr = nullptr);

	UFUNCTION()
	virtual bool CanPerform();

	// Activates the action (double checking if can perform, starts targeting if required), returns activation success
	UFUNCTION(BlueprintCallable, Category = "ETV Action")
	virtual bool Activate();

    UFUNCTION()
	virtual void Perform();

    UFUNCTION()
	virtual void ApplyEffectsSelf() {}

    UFUNCTION()
	virtual EETVActionAvailability ToggleAvailability();


};
