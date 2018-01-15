// Copyright (C) Team13. All rights reserved.

#include "ETVShipCapital.h"
#include "ETVWeaponTorpedo.h"
#include "ETVWeaponRepairArm.h"
#include "ETVWeaponShieldBattery.h"

AETVShipCapital::AETVShipCapital() : Super()
{
	Class = EETVShipClass::Capital;
	SensorRange = 5;
}

void AETVShipCapital::Init(FString NewName, int32 MaxHP, int32 ShieldP, int32 NewShieldRechargeTime, int32 NewMoveRange, int32 Speed, bool bHyperDrive, float NewLaserResistance, int32 Hangars)
{
	Super::Init(NewName, MaxHP, ShieldP, NewShieldRechargeTime, NewMoveRange, Speed);
	bHasHyperdrive = bHyperDrive;
	LaserResistance = NewLaserResistance;
	NumOfHangars = Hangars;
}

void AETVShipCapital::InitRandom(FString NewName)
{
	Level = FMath::RandRange(125, 225);
	InitRandomWithLevel(NewName, Level);
}

void AETVShipCapital::InitRandomWithLevel(FString NewName, int32 PowerLvl)
{
	// Bonus because it is the Capital Ship
	PowerLvl += 25;

	if (PowerLvl < 50)
		PowerLvl = 50;
	else if (PowerLvl > 200)
		PowerLvl = 200;

	Super::InitRandomWithLevel(NewName, PowerLvl);

	// Only one Capital per side, so both get Hyperdrive
	bHasHyperdrive = true;

	LaserResistance = FMath::RandRange(PowerLvl / 500, 0.5);

	// The better the level the lower the time (min 1)
	int32 MinHangars = 200 / PowerLvl;
	int32 MaxHangars = MinHangars + 3;
	NumOfHangars = FMath::RandRange(MinHangars, MaxHangars);

	SpawnWeapons();
}

void AETVShipCapital::SpawnWeapons()
{
	// Actor spawn parameters
	const FActorSpawnParameters SpawnInfo;

	// Rotate upwards to face the top-down camera
	const FRotator Rotator(0, 0, -90);

	// Add Torpedo
	UETVWeaponSlot* WeaponSlotTorpedo = NewObject<UETVWeaponSlot>();
	WeaponSlotTorpedo->Init(Level, Level);

	AETVWeaponTorpedo* Torpedo;
	Torpedo = GetWorld()->SpawnActor<AETVWeaponTorpedo>(PositionInWorld, Rotator, SpawnInfo);

	do {
		Torpedo->InitRandom("Torpedo", Level);
	} while (!WeaponSlotTorpedo->DoesWeaponFit(Torpedo));
	WeaponSlotTorpedo->FitWeapon(Torpedo);
	AddWeapon(WeaponSlotTorpedo);

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
