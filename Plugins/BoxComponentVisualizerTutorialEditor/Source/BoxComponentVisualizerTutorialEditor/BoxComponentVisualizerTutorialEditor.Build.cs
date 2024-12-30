// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class BoxComponentVisualizerTutorialEditor : ModuleRules
{
	public BoxComponentVisualizerTutorialEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicDependencyModuleNames.AddRange(
            new string[] { 
                "Core", 
                "CoreUObject", 
                "Engine", 
                "InputCore" 
            });

        PrivateDependencyModuleNames.AddRange(
            new string[] { 
                "UnrealEd", 
                "ComponentVisualizers" 
            });
	}
}
