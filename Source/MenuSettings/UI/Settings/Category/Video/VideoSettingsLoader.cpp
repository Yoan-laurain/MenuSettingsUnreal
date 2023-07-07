#include "MenuSettings/UI/Settings/LocalSettings.h"
#include "MenuSettings/UI/Settings/Category/GameSettingsCollection.h"
#include "MenuSettings/UI/Settings/Category/SettingsManager.h"

template<typename TypeOption>
TArray<TypeOption> UGameSettingsItem::TechnicalOption;

UGameSettingsCollection* USettingsManager::InitializeVideoSettings()
{
	ULocalSettings* LocalSettings = ULocalSettings::Get();
	UGameSettingsCollection* Screen = NewObject<UGameSettingsCollection>();
	Screen->SetTitle(FText::FromString("Video"));

	TArray<float> GenericQualityOptions;
			
	for (int32 i = 0; i < 5; ++i)
	{
		GenericQualityOptions.Add(i);
	}
	
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
			
			WindowModeItem->GetCurrentOptionValueDelegate().BindLambda( [LocalSettings,WindowModeItem] ()
			{
				LocalSettings->SetFullscreenMode(WindowModeItem->GetTechnicalOption<EWindowMode::Type>());
			} );
				
			WindowModeItem->ClearOptions();

			// For screen
			WindowModeItem->AddOption( FText::FromString("Fullscreen"));
			WindowModeItem->AddOption( FText::FromString("Windowed Fullscreen"));
			WindowModeItem->AddOption( FText::FromString("Windowed"));

			// For technical
			TArray<EWindowMode::Type> WindowModeOptions;
			WindowModeOptions.Add(EWindowMode::Fullscreen);
			WindowModeOptions.Add(EWindowMode::WindowedFullscreen);
			WindowModeOptions.Add(EWindowMode::Windowed);
			
			WindowModeItem->SetTechnicalOption(WindowModeOptions);
			
			int Index = WindowModeOptions.Find(LocalSettings->GetFullscreenMode());
			WindowModeItem->SetIndexCurrentOption(Index);
			
			Display->AddSetting(WindowModeItem);
		}
		//----------------------------------------------------------------------------------
		{
			UGameSettingsItem* ResolutionItem = NewObject<UGameSettingsItem>();
			ResolutionItem->SetOptionName(FText::FromString("Resolution"));
			ResolutionItem->SetDescriptionRichText(FText::FromString(
				"Display Resolution determines the size of the window in Windowed mode. In Fullscreen mode, Display Resolution determines the graphics card output resolution, which can result in black bars depending on monitor and graphics card. Display Resolution is inactive in Windowed Fullscreen mode."));

			ResolutionItem->GetCurrentOptionValueDelegate().BindLambda( [LocalSettings,ResolutionItem] ()
			{
				LocalSettings->SetScreenResolution(ResolutionItem->GetTechnicalOption<FIntPoint>());
			} );

			ResolutionItem->ClearOptions();
			ResolutionItem->AddOption(FText::FromString("1280x720"));
			ResolutionItem->AddOption(FText::FromString("1920x1080"));
			ResolutionItem->AddOption(FText::FromString("2560x1440"));
			ResolutionItem->AddOption(FText::FromString("3840x2160"));

			TArray<FIntPoint> ResolutionOptions;
			ResolutionOptions.Add(FIntPoint(1280, 720));
			ResolutionOptions.Add(FIntPoint(1920, 1080));
			ResolutionOptions.Add(FIntPoint(2560, 1440));
			ResolutionOptions.Add(FIntPoint(3840, 2160));

			ResolutionItem->SetTechnicalOption(ResolutionOptions);
			ResolutionItem->SetIndexCurrentOption(ResolutionOptions.Find(LocalSettings->GetScreenResolution()));
			
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
			
			GraphicsItem->GetCurrentOptionValueDelegate().BindLambda( [LocalSettings,GraphicsItem] ()
			{
				LocalSettings->SetOverallScalabilityLevel(GraphicsItem->GetTechnicalOption<int32>());
			});
			
			GraphicsItem->AddOption(FText::FromString("Custom"));

			TArray<int32> QualityOptions;

			for (int32 i = 0; i < 6; ++i)
			{
				QualityOptions.Add(i);
			}

			GraphicsItem->SetTechnicalOption(QualityOptions);
			
			const int Index = LocalSettings->GetOverallScalabilityLevel();
			GraphicsItem->SetIndexCurrentOption(Index == -1 ? 5 : Index);
			
			GraphicsQuality->AddSetting(GraphicsItem);
		}
		
		//----------------------------------------------------------------------------------
		{
			UGameSettingsItem* FrameRateLimitItem = NewObject<UGameSettingsItem>();
			FrameRateLimitItem->SetOptionName(FText::FromString("Frame Rate Limit"));
			FrameRateLimitItem->SetDescriptionRichText(FText::FromString("Select a desired framerate. Use this to fine tune performance on your device."));

			FrameRateLimitItem->GetCurrentOptionValueDelegate().BindLambda( [LocalSettings,FrameRateLimitItem] ()
			{
				LocalSettings->SetFrameRateLimit(FrameRateLimitItem->GetTechnicalOption<float>());
			});

			FrameRateLimitItem->ClearOptions();
			for (int32 i = 0; i < 5; ++i)
			{
				FrameRateLimitItem->AddOption(FText::FromString(FString::Printf(TEXT("%d"), 30 * i )));
			}

			TArray<float> FrameRateLimitOptions;
			FrameRateLimitOptions.Add(0.0f);
			FrameRateLimitOptions.Add(30.0f);
			FrameRateLimitOptions.Add(60.0f);
			FrameRateLimitOptions.Add(90.0f);
			FrameRateLimitOptions.Add(120.0f);

			FrameRateLimitItem->SetTechnicalOption(FrameRateLimitOptions);
			FrameRateLimitItem->SetIndexCurrentOption(LocalSettings->GetFrameRateLimit());
			
			GraphicsQuality->AddSetting(FrameRateLimitItem);
		}
		//----------------------------------------------------------------------------------

		//----------------------------------------------------------------------------------
		{
			UGameSettingsItem* AutoSetQuality = NewObject<UGameSettingsItem>();
			AutoSetQuality->SetOptionName(FText::FromString("Auto-Set Quality"));
			AutoSetQuality->SetDescriptionRichText(FText::FromString("Automatically configure the graphics quality options based on a benchmark of the hardware."));

			// TODO : Implement this
		
			AutoSetQuality->GetCurrentOptionValueDelegate().BindLambda( [=] ()
			{
				LocalSettings->RunAutoBenchmark(false);
			});
			
			AutoSetQuality->ClearOptions();
			AutoSetQuality->AddOption(FText::FromString("On"));
			AutoSetQuality->AddOption(FText::FromString("Off"));
			
			TArray<bool> AutoSetQualityOptions;
			AutoSetQualityOptions.Add(true);
			AutoSetQualityOptions.Add(false);

			AutoSetQuality->SetTechnicalOption(AutoSetQualityOptions);
			AutoSetQuality->SetIndexCurrentOption(1);
			
			GraphicsQuality->AddSetting(AutoSetQuality);
		}
		//----------------------------------------------------------------------------------
		
		//----------------------------------------------------------------------------------
		{
			UGameSettingsItem* LightingQualityItem = NewObject<UGameSettingsItem>();
			LightingQualityItem->SetOptionName(FText::FromString("Lighting Quality"));
			LightingQualityItem->SetDescriptionRichText(FText::FromString("Lighting Quality affects the quality of lighting and shadows. Lower settings can improve performance."));
			
			LightingQualityItem->GetCurrentOptionValueDelegate().BindLambda( [LocalSettings,LightingQualityItem] ()
			{
				LocalSettings->SetGlobalIlluminationQuality(LightingQualityItem->GetTechnicalOption<int>() );
			} );
			
			LightingQualityItem->SetTechnicalOption(GenericQualityOptions);
			LightingQualityItem->SetIndexCurrentOption(LocalSettings->GetGlobalIlluminationQuality());
			
			GraphicsQuality->AddSetting(LightingQualityItem);
		}
		//----------------------------------------------------------------------------------

		//----------------------------------------------------------------------------------
		{
			UGameSettingsItem* ShadowsItem = NewObject<UGameSettingsItem>();
			ShadowsItem->SetOptionName(FText::FromString("Shadows"));
			ShadowsItem->SetDescriptionRichText(FText::FromString("Shadows affect the quality of shadows cast by lights. Lower settings can improve performance."));
			
			ShadowsItem->GetCurrentOptionValueDelegate().BindLambda( [LocalSettings,ShadowsItem] ()
			{
				LocalSettings->SetShadowQuality(ShadowsItem->GetTechnicalOption<int>());
			} );
			
			ShadowsItem->SetTechnicalOption(GenericQualityOptions);
			ShadowsItem->SetIndexCurrentOption( LocalSettings->GetShadowQuality() );
			
			GraphicsQuality->AddSetting(ShadowsItem);
		}
		//----------------------------------------------------------------------------------

		//----------------------------------------------------------------------------------
		{
			UGameSettingsItem* AntiAliasingItem = NewObject<UGameSettingsItem>();
			AntiAliasingItem->SetOptionName(FText::FromString("Anti-Aliasing"));
			AntiAliasingItem->SetDescriptionRichText(FText::FromString("Anti-Aliasing affects the smoothness of edges. Lower settings can improve performance."));
			
			AntiAliasingItem->GetCurrentOptionValueDelegate().BindLambda( [LocalSettings,AntiAliasingItem] ()
			{
				LocalSettings->SetAntiAliasingQuality(AntiAliasingItem->GetTechnicalOption<int>());
			} );
			
			AntiAliasingItem->SetTechnicalOption(GenericQualityOptions);
			AntiAliasingItem->SetIndexCurrentOption(LocalSettings->GetAntiAliasingQuality());
				
			GraphicsQuality->AddSetting(AntiAliasingItem);
		}
		//----------------------------------------------------------------------------------

		//----------------------------------------------------------------------------------
		{
			UGameSettingsItem* ViewDistanceItem = NewObject<UGameSettingsItem>();
			ViewDistanceItem->SetOptionName(FText::FromString("View Distance"));
			ViewDistanceItem->SetDescriptionRichText(FText::FromString("View Distance affects the distance at which objects are drawn. Lower settings can improve performance."));
			
			ViewDistanceItem->GetCurrentOptionValueDelegate().BindLambda( [LocalSettings,ViewDistanceItem] ()
			{
				LocalSettings->SetViewDistanceQuality(ViewDistanceItem->GetTechnicalOption<int>());
			} );
			
			ViewDistanceItem->SetTechnicalOption(GenericQualityOptions);
			ViewDistanceItem->SetIndexCurrentOption(LocalSettings->GetViewDistanceQuality());
			
			GraphicsQuality->AddSetting(ViewDistanceItem);
		}
		//----------------------------------------------------------------------------------
		
		//----------------------------------------------------------------------------------
		{
			UGameSettingsItem* TexturesItem = NewObject<UGameSettingsItem>();
			TexturesItem->SetOptionName(FText::FromString("Textures"));
			TexturesItem->SetDescriptionRichText(FText::FromString("Texture quality determines the resolution of textures in game. Increasing this setting will make objects more detailed, but can reduce performance."));
			
			TexturesItem->GetCurrentOptionValueDelegate().BindLambda( [LocalSettings,TexturesItem] ()
			{
				LocalSettings->SetTextureQuality(TexturesItem->GetTechnicalOption<int>());
			});
			
			TexturesItem->SetTechnicalOption(GenericQualityOptions);
			TexturesItem->SetIndexCurrentOption(LocalSettings->GetTextureQuality());
			
			GraphicsQuality->AddSetting(TexturesItem);
		}
		//----------------------------------------------------------------------------------

		//----------------------------------------------------------------------------------
		{
			UGameSettingsItem* EffectItem = NewObject<UGameSettingsItem>();
			EffectItem->SetOptionName(FText::FromString("Effects"));
			EffectItem->SetDescriptionRichText(FText::FromString("Effects quality affects the quality of particle effects. Lower settings can improve performance."));
			
			EffectItem->GetCurrentOptionValueDelegate().BindLambda( [LocalSettings,EffectItem] ()
			{
				LocalSettings->SetVisualEffectQuality(EffectItem->GetTechnicalOption<int>());
			} );
			
			EffectItem->SetTechnicalOption(GenericQualityOptions);
			EffectItem->SetIndexCurrentOption(LocalSettings->GetVisualEffectQuality());
			
			GraphicsQuality->AddSetting(EffectItem);
		}
		//----------------------------------------------------------------------------------

		//----------------------------------------------------------------------------------
		{
			UGameSettingsItem* ReflectionsItem = NewObject<UGameSettingsItem>();
			ReflectionsItem->SetOptionName(FText::FromString("Reflections"));
			ReflectionsItem->SetDescriptionRichText(FText::FromString("Reflection quality determines the resolution and accuracy of reflections.  Settings of 'High' and above use more accurate ray tracing methods to solve reflections, but can reduce performance."));
			
			ReflectionsItem->GetCurrentOptionValueDelegate().BindLambda( [LocalSettings,ReflectionsItem] ()
			{
				LocalSettings->SetReflectionQuality(ReflectionsItem->GetTechnicalOption<int>());
			} );

			ReflectionsItem->SetTechnicalOption(GenericQualityOptions);
			ReflectionsItem->SetIndexCurrentOption(LocalSettings->GetReflectionQuality());
			GraphicsQuality->AddSetting(ReflectionsItem);
		}
		//----------------------------------------------------------------------------------
		
		//----------------------------------------------------------------------------------
		{
			UGameSettingsItem* PostProcessingItem = NewObject<UGameSettingsItem>();
			PostProcessingItem->SetOptionName(FText::FromString("Post Processing"));
			PostProcessingItem->SetDescriptionRichText(FText::FromString("Post Processing effects include Motion Blur, Depth of Field and Bloom. Increasing this setting improves the quality of post process effects, but can reduce performance."));
			
			PostProcessingItem->GetCurrentOptionValueDelegate().BindLambda( [LocalSettings,PostProcessingItem] ()
			{
				LocalSettings->SetPostProcessingQuality(PostProcessingItem->GetTechnicalOption<int>());
			} );
			
			PostProcessingItem->SetTechnicalOption(GenericQualityOptions);
			PostProcessingItem->SetIndexCurrentOption(LocalSettings->GetPostProcessingQuality());
			
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
			
			VerticalSyncItem->GetCurrentOptionValueDelegate().BindLambda( [LocalSettings,VerticalSyncItem] ()
			{
				LocalSettings->SetVSyncEnabled(VerticalSyncItem->GetTechnicalOption<float>() > 0.0f);
			});

			VerticalSyncItem->ClearOptions();
			VerticalSyncItem->AddOption(FText::FromString("Off"));
			VerticalSyncItem->AddOption(FText::FromString("On"));

			TArray<float> VerticalSyncOptions;
			VerticalSyncOptions.Add(0.0f);
			VerticalSyncOptions.Add(1.0f);
			
			VerticalSyncItem->SetTechnicalOption(VerticalSyncOptions);
			VerticalSyncItem->SetIndexCurrentOption(LocalSettings->IsVSyncEnabled() ? 1 : 0);
			
			AdvancedGraphics->AddSetting(VerticalSyncItem);
		}
		//----------------------------------------------------------------------------------
	}
	
	return Screen;
}