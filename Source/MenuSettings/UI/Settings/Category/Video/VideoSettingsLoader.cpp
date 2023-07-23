#include "../../LocalSettings.h"
#include "../GameSettingsCollection.h"
#include "../SettingsManager.h"

#define LOCTEXT_NAMESPACE "MySettings"

UGameSettingsCollection* USettingsManager::InitializeVideoSettings()
{
	ULocalSettings* LocalSettings = ULocalSettings::Get();
	UGameSettingsCollection* Screen = NewObject<UGameSettingsCollection>();
	Screen->SetTitle(LOCTEXT("VideoCollection_Name", "Video"));

	TArray<int32> GenericQualityOptions;
			
	for (int32 i = 0; i < 5; ++i)
	{
		GenericQualityOptions.Add(i);
	}

	TArray<UGameSettingsItem*> ParentOptions;
	
	// Display
	////////////////////////////////////////////////////////////////////////////////////
	{
		UGameSettingsCollection* Display = NewObject<UGameSettingsCollection>();
		Display->SetTitle(LOCTEXT("DisplayCollection_Name", "Display"));
		Screen->AddSettingCollection(Display);

		//----------------------------------------------------------------------------------
		{
			UGameSettingsItem* WindowModeItem = NewObject<UGameSettingsItem>();
			WindowModeItem->SetOptionName( LOCTEXT("WindowMode_Name", "Window Mode"));
			WindowModeItem->SetDescriptionRichText(LOCTEXT("WindowMode_Description", "In Windowed mode you can interact with other windows more easily, and drag the edges of the window to set the size. In Windowed Fullscreen mode you can easily switch between applications. In Fullscreen mode you cannot interact with other windows as easily, but the game will run slightly faster."));

			WindowModeItem->SetType(ESettingsType::Normal);
			
			WindowModeItem->GetCurrentOptionValueDelegate().BindLambda( [LocalSettings,WindowModeItem] ()
			{
				int Index = WindowModeItem->GetIndexCurrentOption();
				const EWindowMode::Type WindowMode = static_cast<EWindowMode::Type>(Index);
				LocalSettings->SetFullscreenMode(WindowMode);
			} );
				
			WindowModeItem->ClearOptions();

			// For screen
			WindowModeItem->AddOption( LOCTEXT("Fullscreen_Name", "Fullscreen"));
			WindowModeItem->AddOption( LOCTEXT("WindowedFullscreen_Name", "Windowed Fullscreen"));
			WindowModeItem->AddOption( LOCTEXT("Windowed_Name", "Windowed"));

			// For technical
			TArray<int> WindowModeOptions;
			WindowModeOptions.Add(0);
			WindowModeOptions.Add(1);
			WindowModeOptions.Add(2);
			
			WindowModeItem->SetTechnicalOption(WindowModeOptions);
			
			int Index = WindowModeOptions.Find(LocalSettings->GetFullscreenMode());
			WindowModeItem->SetIndexCurrentOption(Index);
			WindowModeItem->SetDefaultOption(1);
			WindowModeItem->SetMethodToGetIndexFromFile([LocalSettings]() { return LocalSettings->GetFullscreenMode(); });
			
			Display->AddSetting(WindowModeItem);
		}
		//----------------------------------------------------------------------------------
		{
			UGameSettingsItem* ResolutionItem = NewObject<UGameSettingsItem>();
			ResolutionItem->SetOptionName(LOCTEXT("Resolution_Name", "Resolution"));
			ResolutionItem->SetDescriptionRichText(LOCTEXT("Resolution_Description", "Display Resolution determines the size of the window in Windowed mode. In Fullscreen mode, Display Resolution determines the graphics card output resolution, which can result in black bars depending on monitor and graphics card. Display Resolution is inactive in Windowed Fullscreen mode."));

			ResolutionItem->SetType(ESettingsType::Normal);
			
			ResolutionItem->GetCurrentOptionValueDelegate().BindLambda( [LocalSettings,ResolutionItem] ()
			{
				const FIntPoint Resolution = ResolutionItem->ConvertIntToFIntPoint(ResolutionItem->GetIndexCurrentOption());
				LocalSettings->SetScreenResolution(Resolution);
			} );

			ResolutionItem->ClearOptions();
			ResolutionItem->AddOption(FText::FromString("1280x720"));
			ResolutionItem->AddOption(FText::FromString("1920x1080"));
			ResolutionItem->AddOption(FText::FromString("2560x1440"));
			ResolutionItem->AddOption(FText::FromString("3840x2160"));
			
			TArray<int> ResolutionOptions;
			ResolutionOptions.Add(0);
			ResolutionOptions.Add(1);
			ResolutionOptions.Add(2);
			ResolutionOptions.Add(3);
			
			ResolutionItem->SetTechnicalOption(ResolutionOptions);
			int Index = ResolutionItem->ConvertFIntPointToInt(LocalSettings->GetScreenResolution());
			ResolutionItem->SetIndexCurrentOption(Index);
			ResolutionItem->SetDefaultOption(1);
			ResolutionItem->SetMethodToGetIndexFromFile([LocalSettings,ResolutionItem]() { return ResolutionItem->ConvertFIntPointToInt(LocalSettings->GetScreenResolution()); });
			
			Display->AddSetting(ResolutionItem);
		}
		//----------------------------------------------------------------------------------
	}

	// Graphics Quality
	////////////////////////////////////////////////////////////////////////////////////
	{
		UGameSettingsCollection* GraphicsQuality = NewObject<UGameSettingsCollection>();
		GraphicsQuality->SetTitle(LOCTEXT("GraphicsQuality_Name", "Graphics Quality"));
		Screen->AddSettingCollection(GraphicsQuality);

		UGameSettingsItem* GraphicsItem = NewObject<UGameSettingsItem>();
		//----------------------------------------------------------------------------------
		{
			GraphicsItem->SetOptionName(LOCTEXT("DeviceProfileSuffix_Name", "Quality Presets"));
			GraphicsItem->SetDescriptionRichText(LOCTEXT("DeviceProfileSuffix_Description", "Choose between different quality presets to make a trade off between quality and speed."));

			GraphicsItem->SetType(ESettingsType::Normal);
			
			GraphicsItem->GetCurrentOptionValueDelegate().BindLambda( [LocalSettings,GraphicsItem] ()
			{
				LocalSettings->SetOverallScalabilityLevel(GraphicsItem->GetTechnicalOption());
			});
			
			GraphicsItem->AddOption(FText::FromString("Custom"));
			
			TArray<int32> QualityOptions;
			
			for (int32 i = 0; i < 6; ++i)
			{
				QualityOptions.Add(i);
			}
			
			GraphicsItem->SetTechnicalOption(QualityOptions);
			GraphicsItem->SetDefaultOption(2);
			
			const int Index = LocalSettings->GetOverallScalabilityLevel();
			GraphicsItem->SetIndexCurrentOption(Index == -1 ? 5 : Index);
			GraphicsItem->SetBaseOption(Index == -1 ? 5 : Index);
				
			GraphicsItem->SetParentUniqueOption(5);
			
			GraphicsItem->SetMethodToGetIndexFromFile([LocalSettings]() { return LocalSettings->GetOverallScalabilityLevel(); });
			
			ParentOptions.Add(GraphicsItem);
			
			GraphicsQuality->AddSetting(GraphicsItem);
		}
		
		//----------------------------------------------------------------------------------
		{
			UGameSettingsItem* FrameRateLimitItem = NewObject<UGameSettingsItem>();
			FrameRateLimitItem->SetOptionName(LOCTEXT("FrameRateLimit_Mobile_Name", "Frame Rate Limit"));
			FrameRateLimitItem->SetDescriptionRichText(LOCTEXT("FrameRateLimit_Mobile_Description", "Select a desired framerate. Use this to fine tune performance on your device."));

			FrameRateLimitItem->SetType(ESettingsType::Normal);
			
			FrameRateLimitItem->GetCurrentOptionValueDelegate().BindLambda( [LocalSettings,FrameRateLimitItem] ()
			{
				LocalSettings->SetFrameRateLimit(FrameRateLimitItem->GetTechnicalOption());
			});
			
			FrameRateLimitItem->ClearOptions();
			for (int32 i = 0; i < 5; ++i)
			{
				FrameRateLimitItem->AddOption(FText::FromString(FString::Printf(TEXT("%d"), 30 * i )));
			}
			
			TArray<int> FrameRateLimitOptions;
			FrameRateLimitOptions.Add(0);
			FrameRateLimitOptions.Add(30);
			FrameRateLimitOptions.Add(60);
			FrameRateLimitOptions.Add(90);
			FrameRateLimitOptions.Add(120);
			
			FrameRateLimitItem->SetTechnicalOption(FrameRateLimitOptions);
			FrameRateLimitItem->SetDefaultOption(2);
			FrameRateLimitItem->SetIndexCurrentOption(LocalSettings->GetFrameRateLimit() == 0 ? 2 : LocalSettings->GetFrameRateLimit() / 30);
			
			GraphicsQuality->AddSetting(FrameRateLimitItem);
		}
		//----------------------------------------------------------------------------------

		 UGameSettingsItem* AutoSetQuality = NewObject<UGameSettingsItem>();
		 //----------------------------------------------------------------------------------
		 {
		 	AutoSetQuality->SetOptionName(LOCTEXT("AutoSetQuality_Name", "Auto-Set Quality"));
		 	AutoSetQuality->SetDescriptionRichText(LOCTEXT("AutoSetQuality_Description", "Automatically configure the graphics quality options based on a benchmark of the hardware."));

		 	AutoSetQuality->SetType(ESettingsType::Normal);
		
		 	AutoSetQuality->GetCurrentOptionValueDelegate().BindLambda( [=] ()
		 	{
		 		LocalSettings->RunAutoBenchmark(false);
		 	});
		 	
		 	AutoSetQuality->ClearOptions();
		 	AutoSetQuality->AddOption(FText::FromString("Off"));
		 	AutoSetQuality->AddOption(FText::FromString("On"));
		 	
		 	TArray<int> AutoSetQualityOptions;
		 	AutoSetQualityOptions.Add(0);
		 	AutoSetQualityOptions.Add(1);
		
		 	AutoSetQuality->SetTechnicalOption(AutoSetQualityOptions);
		 	AutoSetQuality->SetIndexCurrentOption(0);
		 	AutoSetQuality->SetBaseOption(0);
			AutoSetQuality->SetDefaultOption(0);
		
		 	AutoSetQuality->SetParentUniqueOption(0);
		 	ParentOptions.Add(AutoSetQuality);
		
		 	TArray<UGameSettingsItem*> AutoSetQualityParents;
		 	AutoSetQualityParents.Add(GraphicsItem);
		 	
		 	AutoSetQuality->SetParentOptions(AutoSetQualityParents);
		
		 	TArray<UGameSettingsItem*> GraphicsItemParents;
		 	GraphicsItemParents.Add(AutoSetQuality);
		 	GraphicsItem->SetParentOptions(GraphicsItemParents);
		 	
		 	GraphicsQuality->AddSetting(AutoSetQuality);
		 }
		 //----------------------------------------------------------------------------------
		
		 //----------------------------------------------------------------------------------
		 {
		 	UGameSettingsItem* LightingQualityItem = NewObject<UGameSettingsItem>();
		 	LightingQualityItem->SetOptionName(LOCTEXT("LightingQuality_Name", "Lighting"));
		 	LightingQualityItem->SetDescriptionRichText(LOCTEXT("GlobalIlluminationQuality_Description", "Global Illumination controls the quality of dynamically calculated indirect lighting bounces, sky shadowing and Ambient Occlusion. Settings of 'High' and above use more accurate ray tracing methods to solve lighting, but can reduce performance."));

		 	LightingQualityItem->SetType(ESettingsType::Normal);
		 	
		 	LightingQualityItem->GetCurrentOptionValueDelegate().BindLambda( [LocalSettings,LightingQualityItem] ()
		 	{
		 		LocalSettings->SetGlobalIlluminationQuality(LightingQualityItem->GetTechnicalOption() );
		 	} );
		 	
		 	LightingQualityItem->SetTechnicalOption(GenericQualityOptions);
		 	LightingQualityItem->SetIndexCurrentOption(LocalSettings->GetGlobalIlluminationQuality());
			LightingQualityItem->SetDefaultOption(2);
		
		 	GraphicsItem->AddDependentOption(LightingQualityItem);
		 	AutoSetQuality->AddDependentOption(LightingQualityItem);
		 	LightingQualityItem->SetParentOptions(ParentOptions);
		 	LightingQualityItem->SetMethodToGetIndexFromFile([LocalSettings]() { return LocalSettings->GetGlobalIlluminationQuality(); });
		 	
		 	GraphicsQuality->AddSetting(LightingQualityItem);
		 }
		 //----------------------------------------------------------------------------------
		
		 //----------------------------------------------------------------------------------
		 {
		 	UGameSettingsItem* ShadowsItem = NewObject<UGameSettingsItem>();
		 	ShadowsItem->SetOptionName(LOCTEXT("Shadows_Name", "Shadows"));
		 	ShadowsItem->SetDescriptionRichText(LOCTEXT("Shadows_Description", "Shadow quality determines the resolution and view distance of dynamic shadows. Shadows improve visual quality and give better depth perception, but can reduce performance."));

		 	ShadowsItem->SetType(ESettingsType::Normal);
		 	
		 	ShadowsItem->GetCurrentOptionValueDelegate().BindLambda( [LocalSettings,ShadowsItem] ()
		 	{
		 		LocalSettings->SetShadowQuality(ShadowsItem->GetTechnicalOption());
		 	} );
		 	
		 	ShadowsItem->SetTechnicalOption(GenericQualityOptions);
		 	ShadowsItem->SetIndexCurrentOption( LocalSettings->GetShadowQuality() );
			ShadowsItem->SetDefaultOption(2);
		
		 	GraphicsItem->AddDependentOption(ShadowsItem);
		 	AutoSetQuality->AddDependentOption(ShadowsItem);
		 	ShadowsItem->SetParentOptions(ParentOptions);
		 	ShadowsItem->SetMethodToGetIndexFromFile([LocalSettings]() { return LocalSettings->GetShadowQuality(); });
		 	
		 	GraphicsQuality->AddSetting(ShadowsItem);
		 }
		 //----------------------------------------------------------------------------------
		
		 //----------------------------------------------------------------------------------
		 {
		 	UGameSettingsItem* AntiAliasingItem = NewObject<UGameSettingsItem>();
		 	AntiAliasingItem->SetOptionName(LOCTEXT("AntiAliasing_Name", "Anti-Aliasing"));
		 	AntiAliasingItem->SetDescriptionRichText(LOCTEXT("AntiAliasing_Description", "Anti-Aliasing reduces jaggy artifacts along geometry edges. Increasing this setting will make edges look smoother, but can reduce performance. Higher settings mean more anti-aliasing."));

		 	AntiAliasingItem->SetType(ESettingsType::Normal);
		 	
		 	AntiAliasingItem->GetCurrentOptionValueDelegate().BindLambda( [LocalSettings,AntiAliasingItem] ()
		 	{
		 		LocalSettings->SetAntiAliasingQuality(AntiAliasingItem->GetTechnicalOption());
		 	} );
		 	
		 	AntiAliasingItem->SetTechnicalOption(GenericQualityOptions);
		 	AntiAliasingItem->SetIndexCurrentOption(LocalSettings->GetAntiAliasingQuality());
			AntiAliasingItem->SetDefaultOption(2);
		
		 	GraphicsItem->AddDependentOption(AntiAliasingItem);
		 	AutoSetQuality->AddDependentOption(AntiAliasingItem);
		 	AntiAliasingItem->SetParentOptions(ParentOptions);
		 	AntiAliasingItem->SetMethodToGetIndexFromFile([LocalSettings]() { return LocalSettings->GetAntiAliasingQuality(); });
		 		
		 	GraphicsQuality->AddSetting(AntiAliasingItem);
		 }
		 //----------------------------------------------------------------------------------
		
		 //----------------------------------------------------------------------------------
		 {
		 	UGameSettingsItem* ViewDistanceItem = NewObject<UGameSettingsItem>();
		 	ViewDistanceItem->SetOptionName(LOCTEXT("ViewDistance_Name", "View Distance"));
		 	ViewDistanceItem->SetDescriptionRichText(LOCTEXT("ViewDistance_Description", "View distance determines how far away objects are culled for performance."));

		 	ViewDistanceItem->SetType(ESettingsType::Normal);
		 	
		 	ViewDistanceItem->GetCurrentOptionValueDelegate().BindLambda( [LocalSettings,ViewDistanceItem] ()
		 	{
		 		LocalSettings->SetViewDistanceQuality(ViewDistanceItem->GetTechnicalOption());
		 	} );
		 	
		 	ViewDistanceItem->SetTechnicalOption(GenericQualityOptions);
		 	ViewDistanceItem->SetIndexCurrentOption(LocalSettings->GetViewDistanceQuality());
			ViewDistanceItem->SetDefaultOption(2);
		
		 	GraphicsItem->AddDependentOption(ViewDistanceItem);
		 	AutoSetQuality->AddDependentOption(ViewDistanceItem);
		 	ViewDistanceItem->SetParentOptions(ParentOptions);
		 	ViewDistanceItem->SetMethodToGetIndexFromFile([LocalSettings]() { return LocalSettings->GetViewDistanceQuality(); });
		 	
		 	GraphicsQuality->AddSetting(ViewDistanceItem);
		 }
		 //----------------------------------------------------------------------------------
		
		 //----------------------------------------------------------------------------------
		 {
		 	UGameSettingsItem* TexturesItem = NewObject<UGameSettingsItem>();
		 	TexturesItem->SetOptionName(LOCTEXT("TextureQuality_Name", "Textures"));
		 	TexturesItem->SetDescriptionRichText(LOCTEXT("TextureQuality_Description", "Texture quality determines the resolution of textures in game. Increasing this setting will make objects more detailed, but can reduce performance."));

		 	TexturesItem->SetType(ESettingsType::Normal);
		 	
		 	TexturesItem->GetCurrentOptionValueDelegate().BindLambda( [LocalSettings,TexturesItem] ()
		 	{
		 		LocalSettings->SetTextureQuality(TexturesItem->GetTechnicalOption());
		 	});
		 	
		 	TexturesItem->SetTechnicalOption(GenericQualityOptions);
		 	TexturesItem->SetIndexCurrentOption(LocalSettings->GetTextureQuality());
			TexturesItem->SetDefaultOption(2);
		
		 	GraphicsItem->AddDependentOption(TexturesItem);
		 	AutoSetQuality->AddDependentOption(TexturesItem);
		 	TexturesItem->SetParentOptions(ParentOptions);
		 	TexturesItem->SetMethodToGetIndexFromFile([LocalSettings]() { return LocalSettings->GetTextureQuality(); });
		 	
		 	GraphicsQuality->AddSetting(TexturesItem);
		 }
		 //----------------------------------------------------------------------------------
		
		 //----------------------------------------------------------------------------------
		 {
		 	UGameSettingsItem* EffectItem = NewObject<UGameSettingsItem>();
		 	EffectItem->SetOptionName(LOCTEXT("VisualEffectQuality_Name", "Effects"));
		 	EffectItem->SetDescriptionRichText(LOCTEXT("VisualEffectQuality_Description", "Effects determines the quality of visual effects and lighting in game. Increasing this setting will increase the quality of visual effects, but can reduce performance."));

		 	EffectItem->SetType(ESettingsType::Normal);
		 	
		 	EffectItem->GetCurrentOptionValueDelegate().BindLambda( [LocalSettings,EffectItem] ()
		 	{
		 		LocalSettings->SetVisualEffectQuality(EffectItem->GetTechnicalOption());
		 	} );
		 	
		 	EffectItem->SetTechnicalOption(GenericQualityOptions);
		 	EffectItem->SetIndexCurrentOption(LocalSettings->GetVisualEffectQuality());
			EffectItem->SetDefaultOption(2);
		
		 	GraphicsItem->AddDependentOption(EffectItem);
		 	AutoSetQuality->AddDependentOption(EffectItem);
		 	EffectItem->SetParentOptions(ParentOptions);
		 	EffectItem->SetMethodToGetIndexFromFile([LocalSettings]() { return LocalSettings->GetVisualEffectQuality(); });
		 	
		 	GraphicsQuality->AddSetting(EffectItem);
		 }
		 //----------------------------------------------------------------------------------
		
		 //----------------------------------------------------------------------------------
		 {
		 	UGameSettingsItem* ReflectionsItem = NewObject<UGameSettingsItem>();
		 	ReflectionsItem->SetOptionName(LOCTEXT("ReflectionQuality_Name", "Reflections"));
		 	ReflectionsItem->SetDescriptionRichText(LOCTEXT("ReflectionQuality_Description", "Reflection quality determines the resolution and accuracy of reflections.  Settings of 'High' and above use more accurate ray tracing methods to solve reflections, but can reduce performance."));

		 	ReflectionsItem->SetType(ESettingsType::Normal);
		 	
		 	ReflectionsItem->GetCurrentOptionValueDelegate().BindLambda( [LocalSettings,ReflectionsItem] ()
		 	{
		 		LocalSettings->SetReflectionQuality(ReflectionsItem->GetTechnicalOption());
		 	} );
		
		 	ReflectionsItem->SetTechnicalOption(GenericQualityOptions);
		 	ReflectionsItem->SetIndexCurrentOption(LocalSettings->GetReflectionQuality());
			ReflectionsItem->SetDefaultOption(2);
		
		 	GraphicsItem->AddDependentOption(ReflectionsItem);
		 	AutoSetQuality->AddDependentOption(ReflectionsItem);
		 	ReflectionsItem->SetParentOptions(ParentOptions);
		 	ReflectionsItem->SetMethodToGetIndexFromFile([LocalSettings]() { return LocalSettings->GetReflectionQuality(); });
		 	
		 	GraphicsQuality->AddSetting(ReflectionsItem);
		 }
		 //----------------------------------------------------------------------------------
		
		 //----------------------------------------------------------------------------------
		 {
		 	UGameSettingsItem* PostProcessingItem = NewObject<UGameSettingsItem>();
		 	PostProcessingItem->SetOptionName(LOCTEXT("PostProcessingQuality_Name", "Post Processing"));
		 	PostProcessingItem->SetDescriptionRichText(LOCTEXT("PostProcessingQuality_Description", "Post Processing effects include Motion Blur, Depth of Field and Bloom. Increasing this setting improves the quality of post process effects, but can reduce performance."));  

		 	PostProcessingItem->SetType(ESettingsType::Normal);
		 	
		 	PostProcessingItem->GetCurrentOptionValueDelegate().BindLambda( [LocalSettings,PostProcessingItem] ()
		 	{
		 		LocalSettings->SetPostProcessingQuality(PostProcessingItem->GetTechnicalOption());
		 	} );
		 	
		 	PostProcessingItem->SetTechnicalOption(GenericQualityOptions);
		 	PostProcessingItem->SetIndexCurrentOption(LocalSettings->GetPostProcessingQuality());
			PostProcessingItem->SetDefaultOption(2);
		
		 	GraphicsItem->AddDependentOption(PostProcessingItem);
		 	AutoSetQuality->AddDependentOption(PostProcessingItem);
		 	PostProcessingItem->SetParentOptions(ParentOptions);
		 	PostProcessingItem->SetMethodToGetIndexFromFile([LocalSettings]() { return LocalSettings->GetPostProcessingQuality(); });
		 	
		 	GraphicsQuality->AddSetting(PostProcessingItem);
		 }
		 //----------------------------------------------------------------------------------
	}

	{
		UGameSettingsCollection* AdvancedGraphics = NewObject<UGameSettingsCollection>();
		AdvancedGraphics->SetTitle(LOCTEXT("AdvancedGraphics_Name", "Advanced Graphics"));
		Screen->AddSettingCollection(AdvancedGraphics);

		//----------------------------------------------------------------------------------
		{
			UGameSettingsItem* VerticalSyncItem = NewObject<UGameSettingsItem>();
			VerticalSyncItem->SetOptionName(LOCTEXT("VerticalSync_Name", "Vertical Sync"));
			VerticalSyncItem->SetDescriptionRichText(LOCTEXT("VerticalSync_Description", "Enabling Vertical Sync eliminates screen tearing by always rendering and presenting a full frame. Disabling Vertical Sync can give higher frame rate and better input response, but can result in horizontal screen tearing."));

			VerticalSyncItem->SetType(ESettingsType::Normal);
			
			VerticalSyncItem->GetCurrentOptionValueDelegate().BindLambda( [LocalSettings,VerticalSyncItem] ()
			{
				LocalSettings->SetVSyncEnabled(VerticalSyncItem->GetTechnicalOption() > 0);
			});

			VerticalSyncItem->ClearOptions();
			VerticalSyncItem->AddOption(FText::FromString("Off"));
			VerticalSyncItem->AddOption(FText::FromString("On"));

			TArray<int> VerticalSyncOptions;
			VerticalSyncOptions.Add(0);
			VerticalSyncOptions.Add(1);
			
			VerticalSyncItem->SetTechnicalOption(VerticalSyncOptions);
			VerticalSyncItem->SetIndexCurrentOption(LocalSettings->IsVSyncEnabled() ? 1 : 0);
			VerticalSyncItem->SetDefaultOption(0);
			
			VerticalSyncItem->SetMethodToGetIndexFromFile(	[LocalSettings]() { return LocalSettings->IsVSyncEnabled() ? 1 : 0; });
			
			AdvancedGraphics->AddSetting(VerticalSyncItem);
		}
		//----------------------------------------------------------------------------------
	}
	
	return Screen;
}