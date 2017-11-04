// Copyright (C) Team13. All rights reserved.

#include "ETVActionSelf.h"

// Sets default values
UETVActionSelf::UETVActionSelf(): Super()
{
	// Require 2 tile of distance from any enemy
	RequiredSafetyDistance = 2;
}

bool UETVActionSelf::CanPerform()
{
    return Super::CanPerform() && IsSafe();
}


bool UETVActionSelf::IsSafe()
{
	// TODO Actually check if outside RequiredSafetyDistance
    return true;
}
