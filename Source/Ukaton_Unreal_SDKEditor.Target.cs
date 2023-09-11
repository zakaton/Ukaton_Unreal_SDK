// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class Ukaton_Unreal_SDKEditorTarget : TargetRules
{
	public Ukaton_Unreal_SDKEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new string[] { "Ukaton_Unreal_SDK" } );
	}
}
