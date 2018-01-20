// Copyright (C) Team13. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ETVAI.generated.h"
#include "ETVShip.h"

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
 */
UCLASS()
class ETV_API UETVAI : public UObject
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	UETVAI();
	
private:


public:
	// Returns the move AI will make this turn depending on the given board state
	TArray<int32> GetMove(TArray<AETVShip*> Ships);
	
	
	
};
