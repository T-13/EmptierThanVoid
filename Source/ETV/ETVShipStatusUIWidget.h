// Copyright (C) Team13. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ETVShip.h"
#include "ETVShipStatusUIWidget.generated.h"


/**
 * Class for the ship status reports for the main UI
 */
UCLASS()
class ETV_API UETVShipStatusUIWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	// Pointer to the list of ships in the battle
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ETV ShipsStatusGameUI")
	TArray<AETVShip*> Ships;

public:
	// A method to assign the reference to the list of ships
	void AssignShips(TArray<AETVShip*> ListOfShips);
	
	
	
};
