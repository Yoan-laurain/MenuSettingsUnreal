#include "MenuSettings/UI/Settings/LocalSettings.h"
#include "MenuSettings/UI/Settings/Category/GameSettingsCollection.h"
#include "MenuSettings/UI/Settings/Category/SettingsManager.h"

UGameSettingsCollection* USettingsManager::InitializeVideoSettings()
{
	ULocalSettings* LocalSettings = ULocalSettings::Get();
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
			WindowModeItem->SetOptionName( FText::FromString("Window Mode"));
			WindowModeItem->SetDescriptionRichText(FText::FromString(
				"In Windowed mode you can interact with other windows more easily, and drag the edges of the window to set the size. In Windowed Fullscreen mode you can easily switch between applications. In Fullscreen mode you cannot interact with other windows as easily, but the game will run slightly faster."));
			
			WindowModeItem->SetCurrentOptionValueDelegate().BindLambda( [LocalSettings,WindowModeItem] ()
			{
				LocalSettings->SetFullscreenMode(WindowModeItem->GetCurrentValue<EWindowMode::Type>());
			} );
			
			WindowModeItem->SetDefaultValue( LocalSettings->GetFullscreenMode() );
				
			WindowModeItem->ClearOptions();
			WindowModeItem->AddOption( FText::FromString("Fullscreen"));
			WindowModeItem->AddOption( FText::FromString("Windowed Fullscreen"));
			WindowModeItem->AddOption( FText::FromString("Windowed"));
			
			Display->AddSetting(WindowModeItem);
		}
		//----------------------------------------------------------------------------------
		{
			UGameSettingsItem* ResolutionItem = NewObject<UGameSettingsItem>();
			ResolutionItem->SetOptionName(FText::FromString("Resolution"));
			ResolutionItem->SetDescriptionRichText(FText::FromString(
				"Display Resolution determines the size of the window in Windowed mode. In Fullscreen mode, Display Resolution determines the graphics card output resolution, which can result in black bars depending on monitor and graphics card. Display Resolution is inactive in Windowed Fullscreen mode."));

			// TODO : Implement this
			
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
			GraphicsItem->SetOptionName(FText::FromString("Quality Presets"));
			GraphicsItem->SetDescriptionRichText(FText::FromString("Choose between different quality presets to make a trade off between quality and speed."));
			
			GraphicsItem->SetCurrentOptionValueDelegate().BindLambda( [LocalSettings,GraphicsItem] ()
			{
				LocalSettings->SetDesiredDeviceProfileQualitySuffix(GraphicsItem->GetCurrentValue<FString>());
			} );
			
			GraphicsItem->SetDefaultValue( LocalSettings->GetDesiredDeviceProfileQualitySuffix() );
			
			GraphicsItem->AddOption(FText::FromString("Custom"));
			
			GraphicsQuality->AddSetting(GraphicsItem);
		}
		
		//----------------------------------------------------------------------------------
		{
			UGameSettingsItem* FrameRateLimitItem = NewObject<UGameSettingsItem>();
			FrameRateLimitItem->SetOptionName(FText::FromString("Frame Rate Limit"));
			FrameRateLimitItem->SetDescriptionRichText(FText::FromString("Select a desired framerate. Use this to fine tune performance on your device."));
			
			// TODO : Implement this
			
			GraphicsQuality->AddSetting(FrameRateLimitItem);
		}
		//----------------------------------------------------------------------------------

		//----------------------------------------------------------------------------------
		{
			UGameSettingsItem* AutoSetQuality = NewObject<UGameSettingsItem>();
			AutoSetQuality->SetOptionName(FText::FromString("Auto-Set Quality"));
			AutoSetQuality->SetDescriptionRichText(FText::FromString("Automatically configure the graphics quality options based on a benchmark of the hardware."));

			// TODO : Implement this
			
			GraphicsQuality->AddSetting(AutoSetQuality);
		}
		//----------------------------------------------------------------------------------
		
		//----------------------------------------------------------------------------------
		{
			UGameSettingsItem* LightingQualityItem = NewObject<UGameSettingsItem>();
			LightingQualityItem->SetOptionName(FText::FromString("Lighting Quality"));
			LightingQualityItem->SetDescriptionRichText(FText::FromString("Lighting Quality affects the quality of lighting and shadows. Lower settings can improve performance."));
			
			LightingQualityItem->SetCurrentOptionValueDelegate().BindLambda( [LocalSettings,LightingQualityItem] ()
			{
				LocalSettings->SetGlobalIlluminationQuality(LightingQualityItem->GetCurrentValue<float>() );
			} );
			
			LightingQualityItem->SetDefaultValue( LocalSettings->GetGlobalIlluminationQuality() );
			
			GraphicsQuality->AddSetting(LightingQualityItem);
		}
		//----------------------------------------------------------------------------------

		//----------------------------------------------------------------------------------
		{
			UGameSettingsItem* ShadowsItem = NewObject<UGameSettingsItem>();
			ShadowsItem->SetOptionName(FText::FromString("Shadows"));
			ShadowsItem->SetDescriptionRichText(FText::FromString("Shadows affect the quality of shadows cast by lights. Lower settings can improve performance."));
			
			ShadowsItem->SetCurrentOptionValueDelegate().BindLambda( [LocalSettings,ShadowsItem] ()
			{
				LocalSettings->SetShadowQuality(ShadowsItem->GetCurrentValue<float>());
			} );
			
			ShadowsItem->SetDefaultValue( LocalSettings->GetShadowQuality() );
			
			GraphicsQuality->AddSetting(ShadowsItem);
		}
		//----------------------------------------------------------------------------------

		//----------------------------------------------------------------------------------
		{
			UGameSettingsItem* AntiAliasingItem = NewObject<UGameSettingsItem>();
			AntiAliasingItem->SetOptionName(FText::FromString("Anti-Aliasing"));
			AntiAliasingItem->SetDescriptionRichText(FText::FromString("Anti-Aliasing affects the smoothness of edges. Lower settings can improve performance."));
			
			AntiAliasingItem->SetCurrentOptionValueDelegate().BindLambda( [LocalSettings,AntiAliasingItem] ()
			{
				LocalSettings->SetAntiAliasingQuality(AntiAliasingItem->GetCurrentValue<float>());
			} );
			
			AntiAliasingItem->SetDefaultValue( LocalSettings->GetAntiAliasingQuality() );
				
			GraphicsQuality->AddSetting(AntiAliasingItem);
		}
		//----------------------------------------------------------------------------------

		//----------------------------------------------------------------------------------
		{
			UGameSettingsItem* ViewDistanceItem = NewObject<UGameSettingsItem>();
			ViewDistanceItem->SetOptionName(FText::FromString("View Distance"));
			ViewDistanceItem->SetDescriptionRichText(FText::FromString("View Distance affects the distance at which objects are drawn. Lower settings can improve performance."));
			
			ViewDistanceItem->SetCurrentOptionValueDelegate().BindLambda( [LocalSettings,ViewDistanceItem] ()
			{
				LocalSettings->SetViewDistanceQuality(ViewDistanceItem->GetCurrentValue<float>());
			} );
			
			ViewDistanceItem->SetDefaultValue( LocalSettings->GetViewDistanceQuality() );
			
			GraphicsQuality->AddSetting(ViewDistanceItem);
		}
		//----------------------------------------------------------------------------------
		
		//----------------------------------------------------------------------------------
		{
			UGameSettingsItem* TexturesItem = NewObject<UGameSettingsItem>();
			TexturesItem->SetOptionName(FText::FromString("Textures"));
			TexturesItem->SetDescriptionRichText(FText::FromString("Texture quality determines the resolution of textures in game. Increasing this setting will make objects more detailed, but can reduce performance."));
			
			TexturesItem->SetCurrentOptionValueDelegate().BindLambda( [LocalSettings,TexturesItem] ()
			{
				LocalSettings->SetTextureQuality(TexturesItem->GetCurrentValue<float>());
			} );
			
			TexturesItem->SetDefaultValue( LocalSettings->GetTextureQuality() );
			
			GraphicsQuality->AddSetting(TexturesItem);
		}
		//----------------------------------------------------------------------------------

		//----------------------------------------------------------------------------------
		{
			UGameSettingsItem* EffectItem = NewObject<UGameSettingsItem>();
			EffectItem->SetOptionName(FText::FromString("Effects"));
			EffectItem->SetDescriptionRichText(FText::FromString("Effects quality affects the quality of particle effects. Lower settings can improve performance."));
			
			EffectItem->SetCurrentOptionValueDelegate().BindLambda( [LocalSettings,EffectItem] ()
			{
				LocalSettings->SetVisualEffectQuality(EffectItem->GetCurrentValue<float>());
			} );
			
			EffectItem->SetDefaultValue( LocalSettings->GetVisualEffectQuality() );
			
			GraphicsQuality->AddSetting(EffectItem);
		}
		//----------------------------------------------------------------------------------

		//----------------------------------------------------------------------------------
		{
			UGameSettingsItem* ReflectionsItem = NewObject<UGameSettingsItem>();
			ReflectionsItem->SetOptionName(FText::FromString("Reflections"));
			ReflectionsItem->SetDescriptionRichText(FText::FromString("Reflection quality determines the resolution and accuracy of reflections.  Settings of 'High' and above use more accurate ray tracing methods to solve reflections, but can reduce performance."));
			
			ReflectionsItem->SetCurrentOptionValueDelegate().BindLambda( [LocalSettings,ReflectionsItem] ()
			{
				LocalSettings->SetReflectionQuality(ReflectionsItem->GetCurrentValue<float>());
			} );
			
			ReflectionsItem->SetDefaultValue( LocalSettings->GetReflectionQuality() );
			
			GraphicsQuality->AddSetting(ReflectionsItem);
		}
		//----------------------------------------------------------------------------------
		
		//----------------------------------------------------------------------------------
		{
			UGameSettingsItem* PostProcessingItem = NewObject<UGameSettingsItem>();
			PostProcessingItem->SetOptionName(FText::FromString("Post Processing"));
			PostProcessingItem->SetDescriptionRichText(FText::FromString("Post Processing effects include Motion Blur, Depth of Field and Bloom. Increasing this setting improves the quality of post process effects, but can reduce performance."));
			
			PostProcessingItem->SetCurrentOptionValueDelegate().BindLambda( [LocalSettings,PostProcessingItem] ()
			{
				LocalSettings->SetPostProcessingQuality(PostProcessingItem->GetCurrentValue<float>());
			} );
			
			PostProcessingItem->SetDefaultValue( LocalSettings->GetPostProcessingQuality() );
			
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
			VerticalSyncItem->SetOptionName(FText::FromString("Vertical Sync"));
			VerticalSyncItem->SetDescriptionRichText(FText::FromString(
				"Vertical Sync synchronizes the game's framerate with the monitor's refresh rate. This can reduce screen tearing, but can also reduce performance."));

			VerticalSyncItem->SetCurrentOptionValueDelegate().BindLambda( [LocalSettings,VerticalSyncItem] ()
			{
				LocalSettings->SetVSyncEnabled(VerticalSyncItem->GetCurrentValue<float>() > 0.0f);
			} );
			
			VerticalSyncItem->SetDefaultValue( LocalSettings->IsVSyncEnabled() ? 1.0f : 0.0f );
			
			AdvancedGraphics->AddSetting(VerticalSyncItem);
		}
		//----------------------------------------------------------------------------------
	}
	
	return Screen;
}