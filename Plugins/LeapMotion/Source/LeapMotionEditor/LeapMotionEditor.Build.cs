// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

using System.IO;

namespace UnrealBuildTool.Rules
{
	public class LeapMotionEditor : ModuleRules
    {
        public LeapMotionEditor(TargetInfo Target)
        {

            PrivateIncludePaths.AddRange(
                new string[] {
                    "LeapMotionEditor/Private",
					// ... add other private include paths required here ...
				}
                );

            PublicIncludePaths.AddRange(
                new string[] {
                    "LeapMotionEditor/Public",
					// ... add public include paths required here ...
				}
                );

            PrivateDependencyModuleNames.AddRange(
                new string[]
                {
                    "UnrealEd",
                    "BlueprintGraph",
                    "AnimGraph",
                    "AnimGraphRuntime",
                }
                );

            PublicDependencyModuleNames.AddRange(
                 new string[] 
                 {
                    "Engine",
                    "Core",
                    "CoreUObject",
                    //"InputCore",
                    "LeapMotion",
                 }
            );
            
            CircularlyReferencedDependentModules.AddRange(
                new string[] {
                    "LeapMotion",
                }   
            );
            
        }        
	}
}