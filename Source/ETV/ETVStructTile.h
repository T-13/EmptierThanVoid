// Copyright (C) Team13. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "ETVStructTile.generated.h"

USTRUCT(BlueprintType, DisplayName = "ETV Tile")
struct FETVTile
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile Data")
	int32 X;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile Data")
	int32 Y;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile Data")
	int32 Index;

	FETVTile()
	{
		Invalidate();
	}

	void Invalidate()
	{
		X = -1;
		Y = -1;
		Index = -1;
	}

	void Set(const int32 NewX, const int32 NewY, const int32 NewIndex)
	{
		X = NewX;
		Y = NewY;
		Index = NewIndex;
	}

	void Set(const FETVTile& New)
	{
		Set(New.X, New.Y, New.Index);
	}

	bool IsValid() const
	{
		return X >= 0 && Y >= 0 && Index >= 0;
	}

	bool operator==(FETVTile& rhs) const
	{
		return IsValid() && rhs.IsValid() && X == rhs.X && Y == rhs.Y && Index == rhs.Index;
	}

	bool operator!=(FETVTile& rhs) const
	{
		return !(IsValid() && rhs.IsValid() && X == rhs.X && Y == rhs.Y && Index == rhs.Index);
	}
};

USTRUCT(BlueprintType, DisplayName = "ETV Tile Data")
struct FETVTileData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile Data")
	FVector2D PointLeftTop;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile Data")
	FVector2D PointRightBottom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile Data")
	FETVTile Tile;

	FETVTileData()
	{
		PointLeftTop = FVector2D(0, 0);
		PointRightBottom = FVector2D(0, 0);
		Tile = FETVTile();
	}
};
