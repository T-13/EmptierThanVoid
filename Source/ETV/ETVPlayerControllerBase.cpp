// Copyright (C) Team13. All rights reserved.

#include "ETVPlayerControllerBase.h"

// Sets default values
AETVPlayerControllerBase::AETVPlayerControllerBase()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;
}

// Called to bind functionality to input
void AETVPlayerControllerBase::SetupInputComponent()
{
	Super::SetupInputComponent();

}
