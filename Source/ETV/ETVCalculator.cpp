// Copyright (C) Team13. All rights reserved.

#include "ETVCalculator.h"

void UETVCalculator::CalculateWeaponEffect(AETVShip *User, AETVWeapon *WeaponUsed, AETVShip *Target)
{
	/*Printing for debugging*/
	FString Msg = "User HP: " + FString::FromInt(User->GetHP());
	UE_LOG(LogTemp, Warning, TEXT("%s"), *Msg);
	Msg = "Target HP: " + FString::FromInt(Target->GetHP());
	UE_LOG(LogTemp, Warning, TEXT("%s"), *Msg);
	Msg = "Target Shields: " + FString::FromInt(Target->GetShields());
	UE_LOG(LogTemp, Warning, TEXT("%s"), *Msg);
	/**/

	// Calculate the value of change for this effect
	int32 ChangeValue = User->GetMultiplier()*WeaponUsed->GetDMG();

	// Weapon ignores shields
	if(WeaponUsed->GetType() == AETVWeapon::DamageHull)
	{
		Target->SetHP(Target->GetHP() - ChangeValue);
	}	
	// Weapon can't bypass shields
	else if(WeaponUsed->GetType() == AETVWeapon::DamageShieldThenHull)
	{
		// Calculate the value of shields after the attack
		int32 ShieldValue = Target->GetShields() - ChangeValue;	
		if(ShieldValue <= 0)
		{
			// Calculate the amount of dmg that shields absorbed
			ChangeValue -= Target->GetShields();
			// The rest of the dmg is aplied to the Hull
			Target->SetHP(Target->GetHP() - ChangeValue);
		}
		//Apply the new shield value
		Target->SetShields(ShieldValue);
	}	
	// Weapon is a support type that heals HealthPoints
	else if(WeaponUsed->GetType() == AETVWeapon::HealHull)
	{
		Target->SetHP(Target->GetHP() + ChangeValue);		
	}	
	// Weapon is a support type that heals ShieldPoints
	else if(WeaponUsed->GetType() == AETVWeapon::HealShield)
	{
		Target->SetShields(Target->GetShields() + ChangeValue);
	}

	/*Printing for debugging*/
	FString Msg2 = "User HP: " + FString::FromInt(User->GetHP());
	UE_LOG(LogTemp, Warning, TEXT("%s"), *Msg2);
	Msg2 = "Target HP: " + FString::FromInt(Target->GetHP());
	UE_LOG(LogTemp, Warning, TEXT("%s"), *Msg2);
	Msg2 = "Target Shields: " + FString::FromInt(Target->GetShields());
	UE_LOG(LogTemp, Warning, TEXT("%s"), *Msg2);
	/**/
}

