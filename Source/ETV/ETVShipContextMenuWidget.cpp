// Copyright (C) Team13. All rights reserved.

#include "ETVShipContextMenuWidget.h"

// Sets default values
UETVShipContextMenuWidget::UETVShipContextMenuWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ParentShip = nullptr;
	bCanStayOpen = true;
}

void UETVShipContextMenuWidget::AssignShip(AETVShip *ParamShip)
{
	ParentShip = ParamShip;
}

void UETVShipContextMenuWidget::ShouldNotReOpen()
{
	bCanStayOpen = false;
}
