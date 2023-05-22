#include "MenuSettings/UI/Settings/GameSettingsCollection.h"
#include "MenuSettings/UI/Settings/LocalSettings.h"
#include "MenuSettings/UI/Settings/SettingsManager.h"
#include "MenuSettings/UI/Settings/Scalar/GameSettingValueDiscreteDynamic.h"

UGameSettingsCollection* USettingsManager::InitializeVideoSettings()
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
			UGameSettingValueDiscreteDynamic_Enum* WindowModeItem = NewObject<UGameSettingValueDiscreteDynamic_Enum>();
			WindowModeItem->SetNavigationText( FText::FromString("Window Mode"));
			WindowModeItem->SetDescriptionRichText(FText::FromString(
				"In Windowed mode you can interact with other windows more easily, and drag the edges of the window to set the size. In Windowed Fullscreen mode you can easily switch between applications. In Fullscreen mode you cannot interact with other windows as easily, but the game will run slightly faster."));
			
			WindowModeItem->SetDynamicGetter(GET_LOCAL_SETTINGS_FUNCTION_PATH(GetFullscreenMode));
			WindowModeItem->SetDynamicSetter(GET_LOCAL_SETTINGS_FUNCTION_PATH(SetFullscreenMode));
			
			WindowModeItem->ClearOptions();
			WindowModeItem->AddOption( FText::FromString("Fullscreen"));
			WindowModeItem->AddOption( FText::FromString("Windowed Fullscreen"));
			WindowModeItem->AddOption( FText::FromString("Windowed"));
			
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
			
			GraphicsItem->AddOption(FText::FromString("Custom"));
			
			GraphicsQuality->AddSetting(GraphicsItem);
		}
		
		//----------------------------------------------------------------------------------
		{
			UGameSettingsItem* FrameRateLimitItem = NewObject<UGameSettingsItem>();
			FrameRateLimitItem->SetNavigationText(FText::FromString("Frame Rate Limit"));
			FrameRateLimitItem->SetDescriptionRichText(FText::FromString("Select a desired framerate. Use this to fine tune performance on your device."));
			
;
			
			GraphicsQuality->AddSetting(FrameRateLimitItem);
		}
		//----------------------------------------------------------------------------------

		//----------------------------------------------------------------------------------
		{
			UGameSettingsItem* AutoSetQuality = NewObject<UGameSettingsItem>();
			AutoSetQuality->SetNavigationText(FText::FromString("Auto-Set Quality"));
			AutoSetQuality->SetDescriptionRichText(FText::FromString("Automatically configure the graphics quality options based on a benchmark of the hardware."));

			// Setting->SetCustomAction([](ULocalPlayer* LocalPlayer)
			// {
			// 	const ULyraPlatformSpecificRenderingSettings* PlatformSettings = ULyraPlatformSpecificRenderingSettings::Get();
			// 	if (PlatformSettings->FramePacingMode == ELyraFramePacingMode::MobileStyle)
			// 	{
			// 		ULocalSettings::Get()->ResetToMobileDeviceDefaults();
			// 	}
			// 	else
			// 	{
			// 		const ULocalPlayerCustom* LyraLocalPlayer = CastChecked<ULocalPlayerCustom>(LocalPlayer);
			// 		// We don't save state until users apply the settings.
			// 		constexpr bool bImmediatelySaveState = false;
			// 		LyraLocalPlayer->GetLocalSettings()->RunAutoBenchmark(bImmediatelySaveState);
			// 	}
			// })
			
			GraphicsQuality->AddSetting(AutoSetQuality);
		}
		//----------------------------------------------------------------------------------
		
		//----------------------------------------------------------------------------------
		{
			UGameSettingValueDiscreteDynamic_Number* LightingQualityItem = NewObject<UGameSettingValueDiscreteDynamic_Number>();
			LightingQualityItem->SetNavigationText(FText::FromString("Lighting Quality"));
			LightingQualityItem->SetDescriptionRichText(FText::FromString("Lighting Quality affects the quality of lighting and shadows. Lower settings can improve performance."));

			LightingQualityItem->SetDynamicGetter(GET_LOCAL_SETTINGS_FUNCTION_PATH(GetGlobalIlluminationQuality));
			LightingQualityItem->SetDynamicSetter(GET_LOCAL_SETTINGS_FUNCTION_PATH(SetGlobalIlluminationQuality));
			
			GraphicsQuality->AddSetting(LightingQualityItem);
		}
		//----------------------------------------------------------------------------------

		//----------------------------------------------------------------------------------
		{
			UGameSettingValueDiscreteDynamic_Number* ShadowsItem = NewObject<UGameSettingValueDiscreteDynamic_Number>();
			ShadowsItem->SetNavigationText(FText::FromString("Shadows"));
			ShadowsItem->SetDescriptionRichText(FText::FromString("Shadows affect the quality of shadows cast by lights. Lower settings can improve performance."));

			ShadowsItem->SetDynamicGetter(GET_LOCAL_SETTINGS_FUNCTION_PATH(GetShadowQuality));
			ShadowsItem->SetDynamicSetter(GET_LOCAL_SETTINGS_FUNCTION_PATH(SetShadowQuality));
			
			GraphicsQuality->AddSetting(ShadowsItem);
		}
		//----------------------------------------------------------------------------------

		//----------------------------------------------------------------------------------
		{
			UGameSettingValueDiscreteDynamic_Number* AntiAliasingItem = NewObject<UGameSettingValueDiscreteDynamic_Number>();
			AntiAliasingItem->SetNavigationText(FText::FromString("Anti-Aliasing"));
			AntiAliasingItem->SetDescriptionRichText(FText::FromString("Anti-Aliasing affects the smoothness of edges. Lower settings can improve performance."));

			AntiAliasingItem->SetDynamicGetter(GET_LOCAL_SETTINGS_FUNCTION_PATH(GetAntiAliasingQuality));
			AntiAliasingItem->SetDynamicSetter(GET_LOCAL_SETTINGS_FUNCTION_PATH(SetAntiAliasingQuality));
			
			GraphicsQuality->AddSetting(AntiAliasingItem);
		}
		//----------------------------------------------------------------------------------

		//----------------------------------------------------------------------------------
		{
			UGameSettingValueDiscreteDynamic_Number* ViewDistanceItem = NewObject<UGameSettingValueDiscreteDynamic_Number>();
			ViewDistanceItem->SetNavigationText(FText::FromString("View Distance"));
			ViewDistanceItem->SetDescriptionRichText(FText::FromString("View Distance affects the distance at which objects are drawn. Lower settings can improve performance."));

			ViewDistanceItem->SetDynamicGetter(GET_LOCAL_SETTINGS_FUNCTION_PATH(GetViewDistanceQuality));
			ViewDistanceItem->SetDynamicSetter(GET_LOCAL_SETTINGS_FUNCTION_PATH(SetViewDistanceQuality));
			
			GraphicsQuality->AddSetting(ViewDistanceItem);
		}
		//----------------------------------------------------------------------------------
		
		//----------------------------------------------------------------------------------
		{
			UGameSettingValueDiscreteDynamic_Number* PostProcessingItem = NewObject<UGameSettingValueDiscreteDynamic_Number>();
			PostProcessingItem->SetNavigationText(FText::FromString("Textures"));
			PostProcessingItem->SetDescriptionRichText(FText::FromString("Texture quality determines the resolution of textures in game. Increasing this setting will make objects more detailed, but can reduce performance."));

			PostProcessingItem->SetDynamicGetter(GET_LOCAL_SETTINGS_FUNCTION_PATH(GetTextureQuality));
			PostProcessingItem->SetDynamicSetter(GET_LOCAL_SETTINGS_FUNCTION_PATH(SetTextureQuality));
			
			GraphicsQuality->AddSetting(PostProcessingItem);
		}
		//----------------------------------------------------------------------------------

		//----------------------------------------------------------------------------------
		{
			UGameSettingValueDiscreteDynamic_Number* PostProcessingItem = NewObject<UGameSettingValueDiscreteDynamic_Number>();
			PostProcessingItem->SetNavigationText(FText::FromString("Effects"));
			PostProcessingItem->SetDescriptionRichText(FText::FromString("Effects quality affects the quality of particle effects. Lower settings can improve performance."));

			PostProcessingItem->SetDynamicGetter(GET_LOCAL_SETTINGS_FUNCTION_PATH(GetVisualEffectQuality));
			PostProcessingItem->SetDynamicSetter(GET_LOCAL_SETTINGS_FUNCTION_PATH(SetVisualEffectQuality));
			
			GraphicsQuality->AddSetting(PostProcessingItem);
		}
		//----------------------------------------------------------------------------------

		//----------------------------------------------------------------------------------
		{
			UGameSettingValueDiscreteDynamic_Number* PostProcessingItem = NewObject<UGameSettingValueDiscreteDynamic_Number>();
			PostProcessingItem->SetNavigationText(FText::FromString("Reflections"));
			PostProcessingItem->SetDescriptionRichText(FText::FromString("Reflection quality determines the resolution and accuracy of reflections.  Settings of 'High' and above use more accurate ray tracing methods to solve reflections, but can reduce performance."));

			PostProcessingItem->SetDynamicGetter(GET_LOCAL_SETTINGS_FUNCTION_PATH(GetReflectionQuality));
			PostProcessingItem->SetDynamicSetter(GET_LOCAL_SETTINGS_FUNCTION_PATH(SetReflectionQuality));
			
			GraphicsQuality->AddSetting(PostProcessingItem);
		}
		//----------------------------------------------------------------------------------
		
		//----------------------------------------------------------------------------------
		{
			UGameSettingValueDiscreteDynamic_Number* PostProcessingItem = NewObject<UGameSettingValueDiscreteDynamic_Number>();
			PostProcessingItem->SetNavigationText(FText::FromString("Post Processing"));
			PostProcessingItem->SetDescriptionRichText(FText::FromString("Post Processing effects include Motion Blur, Depth of Field and Bloom. Increasing this setting improves the quality of post process effects, but can reduce performance."));
			
			PostProcessingItem->SetDynamicGetter(GET_LOCAL_SETTINGS_FUNCTION_PATH(GetPostProcessingQuality));
			PostProcessingItem->SetDynamicSetter(GET_LOCAL_SETTINGS_FUNCTION_PATH(SetPostProcessingQuality));
			
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
			UGameSettingValueDiscreteDynamic_Bool* VerticalSyncItem = NewObject<UGameSettingValueDiscreteDynamic_Bool>();
			VerticalSyncItem->SetNavigationText(FText::FromString("Vertical Sync"));
			VerticalSyncItem->SetDescriptionRichText(FText::FromString("Vertical Sync synchronizes the game's framerate with the monitor's refresh rate. This can reduce screen tearing, but can also reduce performance."));

			VerticalSyncItem->SetDynamicGetter(GET_LOCAL_SETTINGS_FUNCTION_PATH(IsVSyncEnabled));
			VerticalSyncItem->SetDynamicSetter(GET_LOCAL_SETTINGS_FUNCTION_PATH(SetVSyncEnabled));
			
			AdvancedGraphics->AddSetting(VerticalSyncItem);
		}
		//----------------------------------------------------------------------------------
	}
	
	return Screen;
}