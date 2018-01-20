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
	AETVGameModeBase* GameMode = Cast<AETVGameModeBase>(GetWorld()->GetAuthGameMode());

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
			if (MostImportantAIShip != -1)
			{
				if (ShipReference->GetScore() > Ships[MostImportantAIShip]->GetScore())
				{
					MostImportantAIShip = index;
				}
			}
			else
			{
				MostImportantAIShip = index;
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
				// Currently fire laser is always 0
				UETVActionTarget_Fire* Laser = Cast<UETVActionTarget_Fire>(ShipReference->GetActions()[0]);
				// TODO - Add check if torped avaliable
				Laser->SetTarget(Ships[MostImportantPlayerShip]);
				if(Laser->CanPerform())
				{
					MoveInstructions.Add(Ships[MostImportantPlayerShip]->GetScore());
					MoveInstructions.Add(jndex);
					MoveInstructions.Add(0);
					MoveInstructions.Add(MostImportantPlayerShip);
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
		MoveAction->SetTarget(nullptr, x, y);
	}
	MoveInstructions.Add(Ships[MostImportantPlayerShip]->GetScore());
	MoveInstructions.Add(RandomAIShip);
	MoveInstructions.Add(Ships[RandomAIShip]->GetActions().Num()-1);
	MoveInstructions.Add(x);
	MoveInstructions.Add(y);
	return MoveInstructions;

};




