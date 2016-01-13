// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

using System.IO;

namespace UnrealBuildTool.Rules
{
	public class LeapMotion : ModuleRules
	{
        private string ModulePath
        {
            get { return Path.GetDirectoryName(RulesCompiler.GetModuleFilename(this.GetType().Name)); }
        }

        private string ThirdPartyPath
        {
            get { return Path.GetFullPath(Path.Combine(ModulePath, "../../ThirdParty/")); }
        }

        private string BinariesPath
        {
            get { return Path.GetFullPath(Path.Combine(ModulePath, "../../Binaries/")); }
        }

        private string LibraryPath
        {
            get { return Path.GetFullPath(Path.Combine(ThirdPartyPath, "LeapSDK","Lib")); }
        }

		public LeapMotion(TargetInfo Target)
		{
			PublicIncludePaths.AddRange(
				new string[] {
					"LeapMotion/Public",
					// ... add public include paths required here ...
				}
				);

			PrivateIncludePaths.AddRange(
				new string[] {
					"LeapMotion/Private",
                    Path.Combine(ThirdPartyPath, "LeapSDK", "Include"),
					// ... add other private include paths required here ...
				}
				);

			PublicDependencyModuleNames.AddRange(
				new string[]
				{
					"Engine",
					"Core",
                    "CoreUObject",
                    "InputCore",
                    "HeadMountedDisplay",
                    "Slate",
                    "SlateCore",
					// ... add other public dependencies that you statically link with here ...
				}
				);

            if (UEBuildConfiguration.bBuildEditor == true)
            {

                PublicDependencyModuleNames.AddRange(
                    new string[] {
                    "LeapMotionEditor",
                    "UnrealEd",
                    "AnimGraph",
                    "AnimGraphRuntime",
                    }
                );

                CircularlyReferencedDependentModules.AddRange(
                    new string[] {
                    "LeapMotionEditor",
                    }
                );
            }
            
            LoadLeapLib(Target);
        }

        public bool LoadLeapLib(TargetInfo Target)
        {
            bool isLibrarySupported = false;

            if ((Target.Platform == UnrealTargetPlatform.Win64) || (Target.Platform == UnrealTargetPlatform.Win32))
            {
                isLibrarySupported = true;

                string PlatformString = (Target.Platform == UnrealTargetPlatform.Win64) ? "Win64" : "Win32";

                PublicAdditionalLibraries.Add(Path.Combine(LibraryPath, PlatformString, "Leap.lib"));
            }
            else if (Target.Platform == UnrealTargetPlatform.Mac){

                isLibrarySupported = true;

                string PlatformString = "Mac";
                PublicAdditionalLibraries.Add(Path.Combine(BinariesPath, PlatformString, "libLeap.dylib"));

            }

            return isLibrarySupported;
        }
	}
}