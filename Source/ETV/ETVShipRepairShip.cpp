// Copyright (C) Team13. All rights reserved.

#include "ETVShipRepairShip.h"

AETVShipRepairShip::AETVShipRepairShip() : Super()
{

}

void AETVShipRepairShip::Init(FName NewName, int32 HP, int32 MaxHP, int32 ShieldP, int32 NewShieldRechargeTime, int32 NewSize, int32 NewMoveRange, int32 Speed, int32 NewRepairPower)
{
	Super::Init(NewName, HP, MaxHP, ShieldP, NewShieldRechargeTime, NewSize, NewMoveRange, Speed);
	RepairPower = NewRepairPower;
}
