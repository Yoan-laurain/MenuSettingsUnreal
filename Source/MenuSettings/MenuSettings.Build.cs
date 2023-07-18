using UnrealBuildTool;

public class MenuSettings : ModuleRules
{
	public MenuSettings(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"GameplayTags",
			"GameFeatures"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { 
			"EnhancedInput", 
			"CommonUI",
			"CommonInput",
			"AudioMixer",
			"AudioModulation",
			"DeveloperSettings",
			"PropertyPath", 
			"ModularGameplay"
		});
	}
}