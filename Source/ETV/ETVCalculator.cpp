// Copyright (C) Team13. All rights reserved.

#include "ETVCalculator.h"
#include "ETVGameModeBase.h"

void UETVCalculator::CalculateWeaponEffect(AETVShip *User, AETVWeapon *WeaponUsed, AETVShip *Target)
{
	/*Printing for debugging
	FString Msg = "User HP: " + FString::FromInt(User->GetHP());
	UE_LOG(LogTemp, Warning, TEXT("%s"), *Msg);
	Msg = "Target HP: " + FString::FromInt(Target->GetHP());
	UE_LOG(LogTemp, Warning, TEXT("%s"), *Msg);
	Msg = "Target Shields: " + FString::FromInt(Target->GetShields());
	UE_LOG(LogTemp, Warning, TEXT("%s"), *Msg);
	/**/

	// For ActionLog
	FString ActionHP = "";
	FString ActionShields = "";

	// Calculate the value of change for this effect
	int32 ChangeValue = User->GetMultiplier()*WeaponUsed->GetDMG();

	// Weapon ignores shields
	if (WeaponUsed->GetType() == AETVWeapon::DamageHull)
	{
		Target->SetHP(Target->GetHP() - ChangeValue);
		ActionHP = "Damaged Hull";
	}
	// Weapon can't bypass shields
	else if (WeaponUsed->GetType() == AETVWeapon::DamageShieldThenHull)
	{
		// Calculate the value of shields after the attack
		int32 ShieldValue = Target->GetShields() - ChangeValue;
		if (ShieldValue <= 0)
		{
			// Calculate the amount of dmg that shields absorbed
			ChangeValue -= Target->GetShields();
			// The rest of the dmg is aplied to the Hull
			Target->SetHP(Target->GetHP() - ChangeValue);

			ActionHP = "Damaged Hull";
		}
		else {
			ActionShields = "Damaged Shields";
		}
		//Apply the new shield value
		Target->SetShields(ShieldValue);
	}
	// Weapon is a support type that heals HealthPoints
	else if (WeaponUsed->GetType() == AETVWeapon::HealHull)
	{
		if (Target->GetHP() != Target->GetMaxHP())
		{
			if ((Target->GetHP() + ChangeValue) > Target->GetMaxHP())
			{
				ChangeValue = Target->GetMaxHP() - Target->GetHP();
			}
			Target->SetHP(Target->GetHP() + ChangeValue);
			ActionHP = "Healed Hull";
		}
	}
	// Weapon is a support type that heals ShieldPoints
	else if (WeaponUsed->GetType() == AETVWeapon::HealShield)
	{
		Target->SetShields(Target->GetShields() + ChangeValue);
		ActionShields = "Healed Shields";
	}

	/*Printing for debugging
	FString Msg2 = "User HP: " + FString::FromInt(User->GetHP());
	UE_LOG(LogTemp, Warning, TEXT("%s"), *Msg2);
	Msg2 = "Target HP: " + FString::FromInt(Target->GetHP());
	UE_LOG(LogTemp, Warning, TEXT("%s"), *Msg2);
	Msg2 = "Target Shields: " + FString::FromInt(Target->GetShields());
	UE_LOG(LogTemp, Warning, TEXT("%s"), *Msg2);
	/**/

	// Log if target visible
	AETVGameModeBase* GameMode = Cast<AETVGameModeBase>(User->GetWorld()->GetAuthGameMode());
	if (GameMode->IsTileVisible(Target->GetTilePosition()))
	{
		// Add new Message to UETVActionLogWidget by calling event NewLogEntry
		UETVActionLogWidget *LogWidget = GameMode->GetLogWidget();
		if (ActionHP != "")
		{
			FString ShipName = User->GetShipName();
			FString ChangeValueS = FString::FromInt(ChangeValue);
			FString TargetName = Target->GetShipName();

			FString Message = ShipName + ";" + ActionHP + ";" + ChangeValueS + ";" + TargetName;
			LogWidget->NewLogEntry(Message);
		}
		if (ActionShields != "")
		{
			FString ShipName = User->GetShipName();
			FString ChangeValueS = FString::FromInt(ChangeValue);
			FString TargetName = Target->GetShipName();

			FString Message = ShipName + ";" + ActionShields + ";" + ChangeValueS + ";" + TargetName;
			LogWidget->NewLogEntry(Message);
		}

		if (Target->GetHP() <= 0)
		{
			FString ShipName = User->GetShipName();
			FString TargetName = Target->GetShipName();

			FString Message = ShipName + ";" + TargetName;
			LogWidget->NewDeathLogEntry(Message);
			GameMode->DestroyShip(Target);
		}
	}
}
