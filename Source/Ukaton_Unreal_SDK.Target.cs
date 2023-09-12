// Copyright (c) 2023 Zack Qattan

using UnrealBuildTool;
using System.Collections.Generic;

public class Ukaton_Unreal_SDKTarget : TargetRules
{
	public Ukaton_Unreal_SDKTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange(new string[] { "Ukaton_Unreal_SDK" });
	}
}
