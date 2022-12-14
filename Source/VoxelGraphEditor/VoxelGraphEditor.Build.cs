// Copyright 2021 Phyronnaz

using System.IO;
using UnrealBuildTool;

public class VoxelGraphEditor : ModuleRules
{
    public VoxelGraphEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        bEnforceIWYU = true;
        bLegacyPublicIncludePaths = false;

#if UE_4_24_OR_LATER
#else
#endif

        PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Public"));
        PrivateIncludePaths.Add(Path.Combine(ModuleDirectory, "Private"));
        
        DynamicallyLoadedModuleNames.AddRange(
            new string[] {
                "AssetRegistry"
            });

        PrivateDependencyModuleNames.AddRange(
            new string[] {
                "Core",
                "CoreUObject",
                "Engine",
                "Voxel",
                "VoxelEditor",
                "VoxelGraph",
                "KismetWidgets",
                "AdvancedPreviewScene",
                "Slate",
                "SlateCore",
                "UnrealEd",
                "InputCore",
                "ApplicationCore",
                "GraphEditor",
                "EditorStyle",
                "Projects",
                "BlueprintGraph",
                "DesktopPlatform",
                "Json",
                "GameProjectGeneration",
                "MessageLog",
                "AppFramework",
                "PropertyEditor",
                "Landscape",
                "SettingsEditor",
#if UE_5_0_OR_LATER
                "EditorFramework",
#endif
#if UE_4_24_OR_LATER
                "ToolMenus"
#endif
            });
    }
}