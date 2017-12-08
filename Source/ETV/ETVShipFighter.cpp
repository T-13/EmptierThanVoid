// Copyright (C) Team13. All rights reserved.

#include "ETVShipFighter.h"

AETVShipFighter::AETVShipFighter() : Super()
{
	Class = EETVShipClass::Fighter;
}

void AETVShipFighter::Init(FName NewName, int32 HP, int32 MaxHP, int32 ShieldP, int32 NewShieldRechargeTime, int32 NewSize, int32 NewMoveRange, int32 Speed, float Deflection, float NewAcceleration)
{
	Super::Init(NewName, HP, MaxHP, ShieldP, NewShieldRechargeTime, NewSize, NewMoveRange, Speed);
	DeflectionChance = Deflection;
	Acceleration = NewAcceleration;
}
