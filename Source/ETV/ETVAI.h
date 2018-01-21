// Copyright (C) Team13. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ETVShip.h"
#include "ETVAI.generated.h"

/**
 * Class that contains all the logic for the AI in the game
 * It passes move instructions to the game mode in a form of An Array of int32 Arrays, where ints represent ship indexes, 
 * action indexesm coordinates or traget ship indexes and the board state if this move is used, 1 array of this ints represents 1 posssible move
 * --------------------------------------------
 * 0: Score
 * 1: Ship index
 * 2: Action index
 * 3: Target ship index/X coordinate
 * 4: Y coordinate
 * --------------------------------------------
 * If array returned contains 4 ints it can be deduced that the last int represnts an index for the target ship else it represents the X coordinate
 * of the board
 * --------------------------------------------
 * Ai will look a certain amount of turns into the future, predict the best moves and choose the move that will 
 * lead to the best outcome in set amount of turns
 * --------------------------------------------
 * It currently does not work like this.
 * Problems to solve: Dynamic Deep Copying of actors or support for dummy properties in actors that AI can use to calculate board states.
 * --------------------------------------------
 * AI currently atack the highes priority target of enemy, heals the ship with highest priority on his sie or moves a ship randomly
 * --------------------------------------------
 */
UCLASS()
class ETV_API UETVAI : public UObject
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	UETVAI();


public:
	// Returns the move AI will make this turn depending on the given board state
	TArray<int32> GetMove(TArray<AETVShip*> Ships);
	
	
	
};
