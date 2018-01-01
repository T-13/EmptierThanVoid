// Copyright (C) Team13. All rights reserved.

#include "ETVShipContextMenuWidget.h"

void UETVShipContextMenuWidget::AssignShip(AETVShip *ParamShip)
{
	ParentShip = ParamShip;
}

void UETVShipContextMenuWidget::ShouldNotReOpen()
{
	bCanStayOpen = false;
}
