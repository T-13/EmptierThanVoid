// Copyright (C) Team13. All rights reserved.

#include "ETVActionLogWidget.h"
#include "Runtime/Core/Public/Misc/OutputDeviceNull.h"

void UETVActionLogWidget::AssignMessage(FString Entry)
{
	// Calling my custom event in Widget Blueprint with name Log and parameter entry
	FOutputDeviceNull ar;
	const FString command = FString::Printf(TEXT("Log %s"), *Entry);
	this->CallFunctionByNameWithArguments(*command, ar, NULL, true);
}
