// Copyright (C) Team13. All rights reserved.

#include "ETVShipFighter.h"
#include "ETVWeaponTorpedo.h"

AETVShipFighter::AETVShipFighter() : Super()
{
	Class = EETVShipClass::Fighter;
	MoveRange = 5;
}

void AETVShipFighter::Init(FString NewName, int32 MaxHP, int32 ShieldP, int32 NewShieldRechargeTime, int32 NewMoveRange, int32 Speed, float Deflection)
{
	Super::Init(NewName, MaxHP, ShieldP, NewShieldRechargeTime, NewMoveRange, Speed);
	DeflectionChance = Deflection;
}

void AETVShipFighter::InitRandom(FString NewName)
{
	Level = FMath::RandRange(50, 200);
	InitRandomWithLevel(NewName, Level);
}

void AETVShipFighter::InitRandomWithLevel(FString NewName, int32 PowerLvl)
{
	if (PowerLvl < 50)
		PowerLvl = 50;
	else if (PowerLvl > 200)
		PowerLvl = 200;

	Super::InitRandomWithLevel(NewName, PowerLvl);

	// DeflectionChance is somewhere between 0 and 0.25
	DeflectionChance = FMath::RandRange(PowerLvl / 800, 0.25);

	SpawnWeapons();
}

void AETVShipFighter::SpawnWeapons()
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
}
