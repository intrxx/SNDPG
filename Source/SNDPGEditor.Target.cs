// Copyright 2026 out of sCope team - intrxx

using UnrealBuildTool;
using System.Collections.Generic;

public class SNDPGEditorTarget : TargetRules
{
	public SNDPGEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new string[] { "SNDPG" } );
	}
}
