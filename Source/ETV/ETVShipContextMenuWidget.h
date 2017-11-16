// Copyright (C) Team13. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ETVShipContextMenuWidget.generated.h"


/**
 * Class for the ContextMenuWidget for ETVShips
 */

class AETVShip;

UCLASS()
class ETV_API UETVShipContextMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	

protected:
	// Pointer to the ship for the ContextMenu
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ETV ShipContextMenu")
	AETVShip *ParentShip;

public:
	UFUNCTION(BlueprintCallable)
	void AssignShip(AETVShip *ParamShip);

	
	
};
