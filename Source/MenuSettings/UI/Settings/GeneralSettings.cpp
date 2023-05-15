#include "GeneralSettings.h"
#include "GameSettingsCollection.h"

UGeneralSettings::UGeneralSettings()
{
}

bool UGeneralSettings::CanRunAutoBenchmark() const
{
	return bSupportsAutomaticVideoQualityBenchmark;
}

bool UGeneralSettings::ShouldRunAutoBenchmarkAtStartup() const
{
	if (!CanRunAutoBenchmark())
	{
		return false;
	}

	if (LastCPUBenchmarkResult != -1)
	{
		// Already run and loaded
		return false;
	}

	return true;
}

void UGeneralSettings::RunAutoBenchmark(bool bSaveImmediately)
{
	RunHardwareBenchmark();
	
	// Always apply, optionally save
	ApplyScalabilitySettings();

	if (bSaveImmediately)
	{
		SaveSettings();
	}
}

void UGeneralSettings::ApplyScalabilitySettings()
{
	Scalability::SetQualityLevels(ScalabilityQuality);
}

UGeneralSettings* UGeneralSettings::Get()
{
	UGeneralSettings* GeneralSettings = Cast<UGeneralSettings>(GEngine->GetGameUserSettings());
	return GeneralSettings;
}

UGameSettingsCollection* UGeneralSettings::InitializeVideoSettings()
{
	UGameSettingsCollection* Screen = NewObject<UGameSettingsCollection>();
	Screen->SetTitle(FText::FromString("Video"));

	// Display
	////////////////////////////////////////////////////////////////////////////////////
	{
		UGameSettingsCollection* Display = NewObject<UGameSettingsCollection>();
		Display->SetTitle(FText::FromString("Display"));
		Screen->AddSetting(Display);

		//----------------------------------------------------------------------------------
		{
			UGameSettingsItem* WindowModeItem = NewObject<UGameSettingsItem>();
			WindowModeItem->SetNavigationText( FText::FromString("Window Mode"));
			WindowModeItem->SetDescriptionRichText(FText::FromString(
				"In Windowed mode you can interact with other windows more easily, and drag the edges of the window to set the size. In Windowed Fullscreen mode you can easily switch between applications. In Fullscreen mode you cannot interact with other windows as easily, but the game will run slightly faster."));

			WindowModeItem->ClearOptions();
			WindowModeItem->AddOption("Fullscreen");
			WindowModeItem->AddOption("Windowed Fullscreen");
			WindowModeItem->AddOption("Windowed");
			
			Screen->AddSetting(WindowModeItem);
		}
		//----------------------------------------------------------------------------------
		{
			UGameSettingsItem* ResolutionItem = NewObject<UGameSettingsItem>();
			ResolutionItem->SetNavigationText(FText::FromString("Resolution"));
			ResolutionItem->SetDescriptionRichText(FText::FromString(
				"Display Resolution determines the size of the window in Windowed mode. In Fullscreen mode, Display Resolution determines the graphics card output resolution, which can result in black bars depending on monitor and graphics card. Display Resolution is inactive in Windowed Fullscreen mode."));
		}
		//----------------------------------------------------------------------------------
	}

	// Graphics Quality
	////////////////////////////////////////////////////////////////////////////////////
	{
		UGameSettingsCollection* GraphicsQuality = NewObject<UGameSettingsCollection>();
		GraphicsQuality->SetTitle(FText::FromString("Graphics Quality"));
		Screen->AddSetting(GraphicsQuality);

		//----------------------------------------------------------------------------------
		{
			UGameSettingsItem* GraphicsItem = NewObject<UGameSettingsItem>();
			GraphicsItem->SetNavigationText(FText::FromString("Quality Presets"));
			GraphicsItem->SetDescriptionRichText(FText::FromString("Choose between different quality presets to make a trade off between quality and speed."));
			
			GraphicsItem->AddOption("Custom");
			
			GraphicsQuality->AddSetting(GraphicsItem);
		}
		
		//----------------------------------------------------------------------------------
		{
			UGameSettingsItem* FrameRateLimitItem = NewObject<UGameSettingsItem>();
			FrameRateLimitItem->SetNavigationText(FText::FromString("Frame Rate Limit"));
			FrameRateLimitItem->SetDescriptionRichText(FText::FromString("Select a desired framerate. Use this to fine tune performance on your device."));

			GraphicsQuality->AddSetting(FrameRateLimitItem);
		}
		//----------------------------------------------------------------------------------

		//----------------------------------------------------------------------------------
		{
			UGameSettingsItem* AutoSetQuality = NewObject<UGameSettingsItem>();
			AutoSetQuality->SetNavigationText(FText::FromString("Auto-Set Quality"));
			AutoSetQuality->SetDescriptionRichText(FText::FromString("Automatically configure the graphics quality options based on a benchmark of the hardware."));

			GraphicsQuality->AddSetting(AutoSetQuality);
		}
		//----------------------------------------------------------------------------------
		
		//----------------------------------------------------------------------------------
		{
			UGameSettingsItem* ViewDistanceItem = NewObject<UGameSettingsItem>();
			ViewDistanceItem->SetNavigationText(FText::FromString("Lighting Quality"));
			ViewDistanceItem->SetDescriptionRichText(FText::FromString("Lighting Quality affects the quality of lighting and shadows. Lower settings can improve performance."));

			GraphicsQuality->AddSetting(ViewDistanceItem);
		}
		//----------------------------------------------------------------------------------

		//----------------------------------------------------------------------------------
		{
			UGameSettingsItem* ShadowsItem = NewObject<UGameSettingsItem>();
			ShadowsItem->SetNavigationText(FText::FromString("Shadows"));
			ShadowsItem->SetDescriptionRichText(FText::FromString("Shadows affect the quality of shadows cast by lights. Lower settings can improve performance."));

			GraphicsQuality->AddSetting(ShadowsItem);
		}
		//----------------------------------------------------------------------------------

		//----------------------------------------------------------------------------------
		{
			UGameSettingsItem* AntiAliasingItem = NewObject<UGameSettingsItem>();
			AntiAliasingItem->SetNavigationText(FText::FromString("Anti-Aliasing"));
			AntiAliasingItem->SetDescriptionRichText(FText::FromString("Anti-Aliasing affects the smoothness of edges. Lower settings can improve performance."));

			GraphicsQuality->AddSetting(AntiAliasingItem);
		}
		//----------------------------------------------------------------------------------

		//----------------------------------------------------------------------------------
		{
			UGameSettingsItem* ViewDistanceItem = NewObject<UGameSettingsItem>();
			ViewDistanceItem->SetNavigationText(FText::FromString("View Distance"));
			ViewDistanceItem->SetDescriptionRichText(FText::FromString("View Distance affects the distance at which objects are drawn. Lower settings can improve performance."));

			GraphicsQuality->AddSetting(ViewDistanceItem);
		}
		//----------------------------------------------------------------------------------
		
		//----------------------------------------------------------------------------------
		{
			UGameSettingsItem* PostProcessingItem = NewObject<UGameSettingsItem>();
			PostProcessingItem->SetNavigationText(FText::FromString("Textures"));
			PostProcessingItem->SetDescriptionRichText(FText::FromString("Texture quality determines the resolution of textures in game. Increasing this setting will make objects more detailed, but can reduce performance."));

			GraphicsQuality->AddSetting(PostProcessingItem);
		}
		//----------------------------------------------------------------------------------

		//----------------------------------------------------------------------------------
		{
			UGameSettingsItem* PostProcessingItem = NewObject<UGameSettingsItem>();
			PostProcessingItem->SetNavigationText(FText::FromString("Effects"));
			PostProcessingItem->SetDescriptionRichText(FText::FromString("Effects quality affects the quality of particle effects. Lower settings can improve performance."));

			GraphicsQuality->AddSetting(PostProcessingItem);
		}
		//----------------------------------------------------------------------------------

		//----------------------------------------------------------------------------------
		{
			UGameSettingsItem* PostProcessingItem = NewObject<UGameSettingsItem>();
			PostProcessingItem->SetNavigationText(FText::FromString("Reflections"));
			PostProcessingItem->SetDescriptionRichText(FText::FromString("Reflection quality determines the resolution and accuracy of reflections.  Settings of 'High' and above use more accurate ray tracing methods to solve reflections, but can reduce performance."));

			GraphicsQuality->AddSetting(PostProcessingItem);
		}
		//----------------------------------------------------------------------------------
		
		//----------------------------------------------------------------------------------
		{
			UGameSettingsItem* PostProcessingItem = NewObject<UGameSettingsItem>();
			PostProcessingItem->SetNavigationText(FText::FromString("Post Processing"));
			PostProcessingItem->SetDescriptionRichText(FText::FromString("Post Processing effects include Motion Blur, Depth of Field and Bloom. Increasing this setting improves the quality of post process effects, but can reduce performance."));

			GraphicsQuality->AddSetting(PostProcessingItem);
		}
		//----------------------------------------------------------------------------------
	}

	{
		UGameSettingsCollection* AdvancedGraphics = NewObject<UGameSettingsCollection>();
		AdvancedGraphics->SetTitle(FText::FromString("Advanced Graphics"));
		Screen->AddSetting(AdvancedGraphics);

		//----------------------------------------------------------------------------------
		{
			UGameSettingsItem* VerticalSyncItem = NewObject<UGameSettingsItem>();
			VerticalSyncItem->SetNavigationText(FText::FromString("Vertical Sync"));
			VerticalSyncItem->SetDescriptionRichText(FText::FromString("Vertical Sync synchronizes the game's framerate with the monitor's refresh rate. This can reduce screen tearing, but can also reduce performance."));

			AdvancedGraphics->AddSetting(VerticalSyncItem);
		}
		//----------------------------------------------------------------------------------
	}
	
	return Screen;
}

void SettingChanged() 
{
	Cast<UGeneralSettings>(GEngine->GetGameUserSettings())->ApplyScalabilitySettings();
}