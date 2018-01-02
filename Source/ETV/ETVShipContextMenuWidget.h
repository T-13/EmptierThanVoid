// Copyright (C) Team13. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ETVShipContextMenuWidget.generated.h"

class AETVShip;

/**
 * Class for the ContextMenuWidget for ETVShips.
 */
UCLASS()
class ETV_API UETVShipContextMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	UETVShipContextMenuWidget(const FObjectInitializer& ObjectInitializer);

protected:
	// Pointer to the ship for the ContextMenu
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ETV ShipContextMenu")
	AETVShip *ParentShip;

	// Tells the context menu if it can stay open
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ETV ShipContextMenu")
	bool bCanStayOpen;

public:
	UFUNCTION(BlueprintCallable)
	void AssignShip(AETVShip *ParamShip);
	
	UFUNCTION(BlueprintCallable)
	void ShouldNotReOpen();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "ETV Click")
	void Close();

	// Custom Tick due to UE-22374
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "ETV Click")
	void Ticked();
};
