// Copyright (C) Team13. All rights reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class ETVTarget : TargetRules
{
	public ETVTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;

		ExtraModuleNames.AddRange( new string[] { "ETV" } );
	}
}
