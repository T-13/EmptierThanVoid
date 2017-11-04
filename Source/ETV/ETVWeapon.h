// Copyright (C) Team13. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "PaperSpriteActor.h"
#include "fvector"
#include "fstring"



#include "ETVWeapon.generated.h"


/**
 * 
 */
UCLASS()
class ETV_API AETVWeapon : public APaperSpriteActor
{
    GENERATED_BODY()
protected:
    UPROPERTY(EditAnywhere)
    fstring Name;

    UPROPERTY(EditAnywhere, meta = (ClampMin = "0.0", ClampMax = "100.0"))
    float HP;

    UPROPERTY(EditAnywhere)
    float Damage;

    UPROPERTY(EditAnywhere)
    fstring Type;

    UPROPERTY(EditAnywhere)
    float Weight;

    UPROPERTY(EditAnywhere)
    float Armour;

    UPROPERTY(EditAnywhere)
    int32 Angle[2];

    UPROPERTY(EditAnywhere)
    int32 ActionPointsPerAction;

    UPROPERTY(EditAnywhere)
    int32 travelDistance;

public:
    // Sets default values for this actor's properties
    AETVWeapon();

    //Method to tell the weapon to fire.
    UFUNCTION(BlueprintCallable)
    void fire(int x, int y);

    //Get targets that weapon can hit
    UFUNCTION(BlueprintCallable)
    fvector<int> getPossibleTargets();   //TODO-Object AETVShip not yet defined 

    //Gives the requirments for the weapon slot
    UFUNCTION(BlueprintCallable)
    int getSlotSpecifications();    //TODO-Object SlotSpecification not yet defined 

    //Get current
    UFUNCTION(BlueprintCallable)
    int getRport(); //TODO-Object REPORT not yet defined 
};
