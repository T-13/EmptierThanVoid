// Copyright (C) Team13. All rights reserved.

#include "ETVActionLogWidget.h"
#include "Runtime/Core/Public/Misc/OutputDeviceNull.h"

void UETVActionLogWidget::AssignMessage(FString entry)
{
	FOutputDeviceNull ar;
	const FString command = FString::Printf(TEXT("Log %s"), *entry);
	this->CallFunctionByNameWithArguments(*command, ar, NULL, true);
}
