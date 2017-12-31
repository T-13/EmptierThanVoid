// Copyright (C) Team13. All rights reserved.

#include "ETVShipCapital.h"


AETVShipCapital::AETVShipCapital() : Super()
{
	Class = EETVShipClass::Capital;
}

void AETVShipCapital::Init(FString NewName, int32 HP, int32 MaxHP, int32 ShieldP, int32 NewShieldRechargeTime, int32 NewSize, int32 NewMoveRange, int32 Speed, bool HyperDrive, float NewLaserResistance, int32 Hangars)
{
	Super::Init(NewName, HP, MaxHP, ShieldP, NewShieldRechargeTime, NewSize, NewMoveRange, Speed);
	HasHyperdrive = HyperDrive;
	LaserResistance = NewLaserResistance;
	NumOfHangars = Hangars;
}
