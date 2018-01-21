// Copyright (C) Team13. All rights reserved.

#include "ETVAI.h"
#include "ETVGameModeBase.h"
#include "ETVActionTarget_Fire.h"
#include "ETVActionTarget_Move.h"

// TODO Returns duplicate objects of all the ships on the board for AI to calculate the next move
TArray<AETVShip*> GetBoardStateClone(TArray<AETVShip*> BoardState)
{
	TArray<AETVShip*> Clones;
	for(auto ShipRef : BoardState)
	{
		// Check for class?

		// Cast and create clone

		// Get clone's reference
		
		// Push reference to clones
		
	}
	return Clones;
}

// TODO Calculates maximum score and the coresponding board state if the given ship makes a move
TArray<AETVShip*> GetNextBoardStateAndScore(TArray<AETVShip*> BoardState, int32 ShipIndex, TArray<TArray<int32>> &MoveInstructions)
{
	return TArray<AETVShip*>();
}

// TODO Returns the 5 best moves for the given boardState
TArray<TArray<AETVShip*>> GetTop5Moves(TArray<AETVShip*> BoardState, TArray<TArray<int32>> &MoveInstructions)
{
	return TArray<TArray<AETVShip*>>();
}


UETVAI::UETVAI()
{
	
}
// TODO Better Implementation
TArray<int32> UETVAI::GetMove(TArray<AETVShip*> Ships)
{
	TArray<int32> MoveInstructions;

	// Get gamemode
	AETVGameModeBase* GameMode = Cast<AETVGameModeBase>(Ships[0]->GetWorld()->GetAuthGameMode());

	int32 MostImportantPlayerShip = -1;
	int32 MostImportantAIShip = -1;
	int32 RandomAIShip = -1;
	// Find most important ships
	int index = 0;
	for(auto ShipReference : Ships)
	{
		// AI Ship
		if(ShipReference->IsEnemy())
		{
			if(MostImportantAIShip != -1)
			{
				// This will always be the capital
				if(ShipReference->GetScore() > Ships[MostImportantAIShip]->GetScore())
				{
					MostImportantAIShip = index;
				}
			}
			else
			{
				MostImportantAIShip = index;
				RandomAIShip = index;
			}
			// 1/8 chance
			if(FMath::RandBool() && FMath::RandBool() && FMath::RandBool())
			{
				RandomAIShip = index;
			}
		}
		else if(GameMode->IsTileVisible(ShipReference->GetTilePosition(), EETVShipType::EnemyShip) == true)
		{
			if (MostImportantPlayerShip != -1)
			{
				if (ShipReference->GetScore() > Ships[MostImportantPlayerShip]->GetScore())
				{
					MostImportantPlayerShip = index;
				}
			}
			else
			{
				MostImportantPlayerShip = index;
			}
		}
		++index;
	}
	// Does AI see enemy ship 
	if(MostImportantPlayerShip != -1)
	{
		int jndex = 0;
		// If possible try and attack
		for (auto ShipReference : Ships)
		{
			if (ShipReference->IsEnemy())
			{
				// Currently fire laser is always 0 and torpedo is 1 on fighters
				int ActionIndex = 0;
				if(ShipReference->GetShipClass() == "Fighter" || ShipReference->GetShipClass() == "Capital")
					ActionIndex = 1;
				UETVActionTarget_Fire* Laser = Cast<UETVActionTarget_Fire>(ShipReference->GetActions()[ActionIndex]);
				// TODO - Add check if torped avaliable
				Laser->SetTarget(Ships[MostImportantPlayerShip]);
				if(Laser->CanPerform())
				{
					MoveInstructions.SetNum(4);
					MoveInstructions[0] = (Ships[MostImportantPlayerShip]->GetScore());
					MoveInstructions[1] = (jndex);
					MoveInstructions[2] = (ActionIndex);
					MoveInstructions[3] = (MostImportantPlayerShip);
					return MoveInstructions;
				}
			}
			++jndex;
		}
	}
	// We we didn't find enemy ship or we can't attack it
	// We move a ship randomly
	// TODO Add checker if we can heal most important ship
	UETVActionTarget_Move* MoveAction = Cast<UETVActionTarget_Move>(Ships[RandomAIShip]->GetActions().Last(0));
	int x = -1;
	int y = -1;
	while(!MoveAction->CanPerform())
	{
		x = Ships[RandomAIShip]->GetTilePosition().X + FMath::RandRange(1, Ships[RandomAIShip]->GetMoveRange());
		y = Ships[RandomAIShip]->GetTilePosition().Y + FMath::RandRange(1, Ships[RandomAIShip]->GetMoveRange());
		MoveAction->SetTarget(GameMode->GetTileMapActor(), x, y);
	}
	MoveInstructions.SetNum(5);
	MoveInstructions[0] = (Ships[RandomAIShip]->GetScore());
	MoveInstructions[1] = (RandomAIShip);
	MoveInstructions[2] = (Ships[RandomAIShip]->GetActions().Num()-1);
	MoveInstructions[3] = (x);
	MoveInstructions[4] = (y);
	return MoveInstructions;
};




