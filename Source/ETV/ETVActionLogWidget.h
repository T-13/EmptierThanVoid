// Copyright (C) Team13. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ETVActionLogWidget.generated.h"

class AETVShip;
class AETVAction;

/**
 * 
 */
UCLASS()
class ETV_API UETVActionLogWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ETV ActionLogWidget")
	FString msg;

public:

	UFUNCTION(BlueprintCallable)
	void AssignMessage(FString Entry);
};
