// Copyright (C) Team13. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "PaperSpriteActor.h"
//#include "vector"
//#include "string"
/////////////////////////////////////////////
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
    FName Name;

    UPROPERTY(EditAnywhere, meta = (ClampMin = "0.0", ClampMax = "100.0"))
    float HP;

    UPROPERTY(EditAnywhere)
    float Damage;

    UPROPERTY(EditAnywhere)
    FString Type;

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
    virtual void Fire(int32 x, int32 y);

    //Get targets that weapon can hit
    UFUNCTION(BlueprintCallable)
    virtual TArray<int32> GetPossibleTargets();   //TODO-Object AETVShip not yet defined 

    //Gives the requirments for the weapon slot
    UFUNCTION(BlueprintCallable)
    virtual int32 GetSlotSpecifications();    //TODO-Object SlotSpecification not yet defined 

    //Get current status
    UFUNCTION(BlueprintCallable)
    virtual int32 GetRport(); //TODO-Object REPORT not yet defined 
};
