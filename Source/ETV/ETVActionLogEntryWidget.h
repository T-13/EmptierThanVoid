// Copyright (C) Team13. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ETVActionLogEntryWidget.generated.h"

/**
 * 
 */
UCLASS()
class ETV_API UETVActionLogEntryWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ETV ActionLogWidget")
	FString msg;

public:
	UFUNCTION(BlueprintCallable)
	void SetMsg(FString Message);
	
};
