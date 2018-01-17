// Copyright (C) Team13. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ETVObstacle.generated.h"

/**
* Obstacle class for restricting line of sight.
*/
UCLASS()
class ETV_API AETVObstacle : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AETVObstacle();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	
};
