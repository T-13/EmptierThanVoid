// Copyright (C) Team13. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ETVShip.h"
#include "ETVWeapon.h"
#include "ETVActionLogWidget.h"
#include "ETVCalculator.generated.h"

/**
 * 
 */
UCLASS()
class ETV_API UETVCalculator : public UObject
{
	GENERATED_BODY()
	
	
public:
	// Calculates weapon effect, applies it to involved ships/weapons and logs to Combat Log
	UFUNCTION(BlueprintCallable)
	static void CalculateWeaponEffect(AETVShip *Atacker, AETVWeapon *WeaponUsed, AETVShip *Target);
	
};
