// Copyright (C) Team13. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "ETVAction.h"
#include "ETVActionSelf.generated.h"

/**
 * Action class for actions performed on asset using the action.
 */
UCLASS(Blueprintable)
class ETV_API UETVActionSelf : public UETVAction
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	UETVActionSelf();

protected:
    // Required distance from any enemy
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ETV Action|Self", meta = (ClampMin = "0.0"))
    int32 RequiredSafetyDistance;

public:
    UFUNCTION()
    virtual bool IsSafe();

    bool CanPerform() override;

	bool Activate() override;
};
