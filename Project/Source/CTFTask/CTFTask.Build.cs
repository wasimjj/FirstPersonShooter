// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class CTFTask : ModuleRules
{
	public CTFTask(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" ,"OnlineSubsystem","OnlineSubsystemSteam","UMG" });
		PrivateDependencyModuleNames.AddRange(new string[]{"Slate","SlateCore"});
	}
}
