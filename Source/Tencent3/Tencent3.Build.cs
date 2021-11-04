// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Tencent3 : ModuleRules
{
	public Tencent3(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange
		(new string[]
			{"Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "UMG", "Json", "JsonUtilities"});
	}
}
