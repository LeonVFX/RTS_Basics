// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class RTS_Basics : ModuleRules
{
	public RTS_Basics(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "NavigationSystem", "AIModule" });
    }
}
