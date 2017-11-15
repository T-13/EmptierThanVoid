// Copyright (C) Team13. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ETVWeapon.h"
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
UCLASS(NotPlaceable, Abstract)
class ETV_API UETVAction : public UObject
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	UETVAction();

protected:
	// Display name of the action
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ETV Action")
	FName Name;
	
	// Minimal size of the asset using the action
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ETV Action", meta = (ClampMin = "0.0"))
	int32 MinAttachSize;

	// Maximal size of the asset using the action
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ETV Action", meta = (ClampMin = "100.0"))
	int32 MaxAttachSize;

	// Minimal range of the action
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ETV Action", meta = (ClampMin = "0.0"))
	int32 MinRange;

	// Maximal range of the action
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ETV Action", meta = (ClampMin = "100.0"))
	int32 MaxRange;

	// Availability of the action (0 = greyed out, 1 = available
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ETV Action", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	EETVActionAvailability Available;

	// Does action end the turn immediately after use
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ETV Action")
	bool bEndsTurn;

	// Required weapon for the action to be available
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ETV Action")
	TSubclassOf<AETVWeapon> RequiredWeapon;

public:
	UFUNCTION()
	virtual bool CanPerform();

    UFUNCTION()
	virtual void Perform();

    UFUNCTION()
	virtual void ApplyEffectsSelf() {}

    UFUNCTION()
	virtual EETVActionAvailability ToggleAvailability();


};
