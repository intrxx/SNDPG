// Copyright 2026 out of sCope team - intrxx

using UnrealBuildTool;
using System.Collections.Generic;

public class SNDPGTarget : TargetRules
{
	public SNDPGTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new string[] { "SNDPG" } );
	}
}
