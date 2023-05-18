#include "SettingsManager.h"
#include "GameSettingsCollection.h"
#include "GeneralSettings.h"
#include "GameFramework/Character.h"

USettingsManager::USettingsManager()
{
	
}

USettingsManager* USettingsManager::Get(ULocalPlayer* Player)
{
	USettingsManager* Registry = FindObject<USettingsManager>(Player, TEXT("SettingsManager"), true);
	if (Registry == nullptr)
	{
		Registry = NewObject<USettingsManager>(Player, TEXT("SettingsManager"));
		Registry->OnInitialize(Player);
	}

	return Registry;
}

void USettingsManager::SaveChanges()
{
	UGeneralSettings* GeneralSettings = UGeneralSettings::Get();

	if ( GeneralSettings )
	{
		GeneralSettings->ApplySettings(false);
		GeneralSettings->SaveSettings();
	}
}

void USettingsManager::OnInitialize(ULocalPlayer* Player)
{
	VideoSettings = InitializeVideoSettings(Player);
	AudioSettings = InitializeAudioSettings();

	SettingsMap.Add("Video", VideoSettings);
	SettingsMap.Add("Audio", AudioSettings);
}

TArray<FString>* USettingsManager::InitializeNavigationsButtons() const
{
	TArray<FString>* NavigationButtons = new TArray<FString>();

	for (auto& Setting : SettingsMap)
	{
		NavigationButtons->Add(Setting.Key);
	}

	return NavigationButtons;
}

UGameSettingsCollection* USettingsManager::InitializeVideoSettings(ULocalPlayer* Player)
{
	UGameSettingsCollection* Screen = NewObject<UGameSettingsCollection>();
	Screen->SetTitle(FText::FromString("Video"));

	// Display
	////////////////////////////////////////////////////////////////////////////////////
	{
		UGameSettingsCollection* Display = NewObject<UGameSettingsCollection>();
		Display->SetTitle(FText::FromString("Display"));
		Screen->AddSettingCollection(Display);

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
			
			Display->AddSetting(WindowModeItem);
		}
		//----------------------------------------------------------------------------------
		{
			UGameSettingsItem* ResolutionItem = NewObject<UGameSettingsItem>();
			ResolutionItem->SetNavigationText(FText::FromString("Resolution"));
			ResolutionItem->SetDescriptionRichText(FText::FromString(
				"Display Resolution determines the size of the window in Windowed mode. In Fullscreen mode, Display Resolution determines the graphics card output resolution, which can result in black bars depending on monitor and graphics card. Display Resolution is inactive in Windowed Fullscreen mode."));

			Display->AddSetting(ResolutionItem);
		}
		//----------------------------------------------------------------------------------
	}

	// Graphics Quality
	////////////////////////////////////////////////////////////////////////////////////
	{
		UGameSettingsCollection* GraphicsQuality = NewObject<UGameSettingsCollection>();
		GraphicsQuality->SetTitle(FText::FromString("Graphics Quality"));
		Screen->AddSettingCollection(GraphicsQuality);

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
		Screen->AddSettingCollection(AdvancedGraphics);

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

UGameSettingsCollection* USettingsManager::InitializeAudioSettings()
{
	UGameSettingsCollection* Screen = NewObject<UGameSettingsCollection>();
	Screen->SetTitle(FText::FromString("Audio"));

	// Volume
	////////////////////////////////////////////////////////////////////////////////////
	{
		UGameSettingsCollection* Volume = NewObject<UGameSettingsCollection>();
		Volume->SetTitle(FText::FromString("Volume"));
		
		Screen->AddSettingCollection(Volume);

		//----------------------------------------------------------------------------------
		{
			UGameSettingsItem* OverallItem = NewObject<UGameSettingsItem>();
			OverallItem->SetNavigationText( FText::FromString("Overall Volume"));
			OverallItem->SetDescriptionRichText(FText::FromString("Adjusts the volume of everything."));

			Volume->AddSetting(OverallItem);
		}
		//----------------------------------------------------------------------------------
		{
			UGameSettingsItem* MusicItem = NewObject<UGameSettingsItem>();
			MusicItem->SetNavigationText( FText::FromString("Music Volume"));
			MusicItem->SetDescriptionRichText(FText::FromString("Adjusts the volume of music."));

			Volume->AddSetting(MusicItem);
		}
		//----------------------------------------------------------------------------------
		{
			UGameSettingsItem* EffectsItem = NewObject<UGameSettingsItem>();
			EffectsItem->SetNavigationText( FText::FromString("Effects Volume"));
			EffectsItem->SetDescriptionRichText(FText::FromString("Adjusts the volume of sound effects."));

			Volume->AddSetting(EffectsItem);
		}
		//----------------------------------------------------------------------------------
		{
			UGameSettingsItem* VoiceItem = NewObject<UGameSettingsItem>();
			VoiceItem->SetNavigationText( FText::FromString("Dialogue"));
			VoiceItem->SetDescriptionRichText(FText::FromString("Adjusts the volume of dialogue for game characters and voice overs."));

			Volume->AddSetting(VoiceItem);
		}
	}

	return Screen;
}