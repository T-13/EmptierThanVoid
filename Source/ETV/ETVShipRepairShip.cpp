// Copyright (C) Team13. All rights reserved.

#include "ETVShipRepairShip.h"
#include "ETVWeaponRepairArm.h"
#include "ETVWeaponShieldBattery.h"

AETVShipRepairShip::AETVShipRepairShip() : Super()
{
	Class = EETVShipClass::Repair;
	SensorRange = 4;
}

void AETVShipRepairShip::Init(FString NewName, int32 MaxHP, int32 ShieldP, int32 NewShieldRechargeTime, int32 NewMoveRange, int32 Speed, int32 NewRepairPower)
{
	Super::Init(NewName, MaxHP, ShieldP, NewShieldRechargeTime, NewMoveRange, Speed);
	RepairPower = NewRepairPower;
}

void AETVShipRepairShip::InitRandom(FString NewName)
{
	Level = FMath::RandRange(50, 200);
	InitRandomWithLevel(NewName, Level);
}

void AETVShipRepairShip::InitRandomWithLevel(FString NewName, int32 PowerLvl)
{
	if (PowerLvl < 50)
		PowerLvl = 50;
	else if (PowerLvl > 200)
		PowerLvl = 200;

	Super::InitRandomWithLevel(NewName, PowerLvl);

	RepairPower = PowerLvl / 5;
}

void AETVShipRepairShip::SpawnWeapons()
{
	// Actor spawn parameters
	const FActorSpawnParameters SpawnInfo;

	// Rotate upwards to face the top-down camera
	const FRotator Rotator(0, 0, -90);

	// Add RepairArm
	UETVWeaponSlot* WeaponSlotRepairArm = NewObject<UETVWeaponSlot>();
	WeaponSlotRepairArm->Init(Level, Level);

	AETVWeaponRepairArm* RepairArm;
	RepairArm = GetWorld()->SpawnActor<AETVWeaponRepairArm>(PositionInWorld, Rotator, SpawnInfo);

	do {
		RepairArm->InitRandom("Repair Arm", Level);
	} while (!WeaponSlotRepairArm->DoesWeaponFit(RepairArm));
	WeaponSlotRepairArm->FitWeapon(RepairArm);
	AddWeapon(WeaponSlotRepairArm);

	// Add ShieldBattery
	UETVWeaponSlot* WeaponSlotShieldBattery = NewObject<UETVWeaponSlot>();
	WeaponSlotShieldBattery->Init(Level, Level);

	AETVWeaponShieldBattery* ShieldBattery;
	ShieldBattery = GetWorld()->SpawnActor<AETVWeaponShieldBattery>(PositionInWorld, Rotator, SpawnInfo);

	do {
		ShieldBattery->InitRandom("Shield Bat", Level); // Abbreviated "Battery" as "Bat" to fit in Context Menu
	} while (!WeaponSlotShieldBattery->DoesWeaponFit(ShieldBattery));
	WeaponSlotShieldBattery->FitWeapon(ShieldBattery);
	AddWeapon(WeaponSlotShieldBattery);
}
