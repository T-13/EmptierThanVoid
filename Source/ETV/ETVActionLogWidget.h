// Copyright (C) Team13. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ETVActionLogWidget.generated.h"

/**
 * 
 */
UCLASS()
class ETV_API UETVActionLogWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent, Category = "ETV ActionLogWidget")
	void NewLogEntry(const FString& Message);

};
