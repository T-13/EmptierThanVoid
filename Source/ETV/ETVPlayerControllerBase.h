// Copyright (C) Team13. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ETVPlayerControllerBase.generated.h"

/**
 * Base Player Controller class.
 */
UCLASS()
class ETV_API AETVPlayerControllerBase : public APlayerController
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AETVPlayerControllerBase();

};
