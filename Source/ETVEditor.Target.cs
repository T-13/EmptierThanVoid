// Copyright (C) Team13. All rights reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class ETVEditorTarget : TargetRules
{
	public ETVEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;

		ExtraModuleNames.AddRange( new string[] { "ETV" } );
	}
}
