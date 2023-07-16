#include "MenuSettings/UI/Settings/LocalSettings.h"
#include "MenuSettings/UI/Settings/Category/GameSettingsCollection.h"
#include "MenuSettings/UI/Settings/Category/SettingsManager.h"

UGameSettingsCollection* USettingsManager::InitializeVideoSettings()
{
	ULocalSettings* LocalSettings = ULocalSettings::Get();
	UGameSettingsCollection* Screen = NewObject<UGameSettingsCollection>();
	Screen->SetTitle(FText::FromString("Video"));

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
		Display->SetTitle(FText::FromString("Display"));
		Screen->AddSettingCollection(Display);

		//----------------------------------------------------------------------------------
		{
			UGameSettingsItem* WindowModeItem = NewObject<UGameSettingsItem>();
			WindowModeItem->SetOptionName( FText::FromString("Window Mode"));
			WindowModeItem->SetDescriptionRichText(FText::FromString(
				"In Windowed mode you can interact with other windows more easily, and drag the edges of the window to set the size. In Windowed Fullscreen mode you can easily switch between applications. In Fullscreen mode you cannot interact with other windows as easily, but the game will run slightly faster."));

			WindowModeItem->SetType(ESettingsType::Normal);
			
			WindowModeItem->GetCurrentOptionValueDelegate().BindLambda( [LocalSettings,WindowModeItem] ()
			{
				int Index = WindowModeItem->GetIndexCurrentOption();
				const EWindowMode::Type WindowMode = static_cast<EWindowMode::Type>(Index);
				LocalSettings->SetFullscreenMode(WindowMode);
			} );
				
			WindowModeItem->ClearOptions();

			// For screen
			WindowModeItem->AddOption( FText::FromString("Fullscreen"));
			WindowModeItem->AddOption( FText::FromString("Windowed Fullscreen"));
			WindowModeItem->AddOption( FText::FromString("Windowed"));

			// For technical
			TArray<int> WindowModeOptions;
			WindowModeOptions.Add(0);
			WindowModeOptions.Add(1);
			WindowModeOptions.Add(2);
			
			WindowModeItem->SetTechnicalOption(WindowModeOptions);
			
			int Index = WindowModeOptions.Find(LocalSettings->GetFullscreenMode());
			WindowModeItem->SetIndexCurrentOption(Index);
			WindowModeItem->SetMethodToGetIndexFromFile([LocalSettings]() { return LocalSettings->GetFullscreenMode(); });
			
			Display->AddSetting(WindowModeItem);
		}
		//----------------------------------------------------------------------------------
		{
			UGameSettingsItem* ResolutionItem = NewObject<UGameSettingsItem>();
			ResolutionItem->SetOptionName(FText::FromString("Resolution"));
			ResolutionItem->SetDescriptionRichText(FText::FromString(
				"Display Resolution determines the size of the window in Windowed mode. In Fullscreen mode, Display Resolution determines the graphics card output resolution, which can result in black bars depending on monitor and graphics card. Display Resolution is inactive in Windowed Fullscreen mode."));

			ResolutionItem->SetType(ESettingsType::Normal);
			
			ResolutionItem->GetCurrentOptionValueDelegate().BindLambda( [LocalSettings,ResolutionItem] ()
			{
				const FIntPoint Resolution = ResolutionItem->ConvertIntToFIntPoint(ResolutionItem->GetIndexCurrentOption());

				UE_LOG(LogTemp, Warning, TEXT("Resolution: %s"), *Resolution.ToString());
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
			ResolutionItem->SetMethodToGetIndexFromFile([LocalSettings,ResolutionItem]() { return ResolutionItem->ConvertFIntPointToInt(LocalSettings->GetScreenResolution()); });
			
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

		UGameSettingsItem* GraphicsItem = NewObject<UGameSettingsItem>();
		//----------------------------------------------------------------------------------
		{
			GraphicsItem->SetOptionName(FText::FromString("Quality Presets"));
			GraphicsItem->SetDescriptionRichText(FText::FromString("Choose between different quality presets to make a trade off between quality and speed."));
			
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
			
			const int Index = LocalSettings->GetOverallScalabilityLevel();
			GraphicsItem->SetIndexCurrentOption(Index == -1 ? 5 : Index);
			GraphicsItem->SetBaseOption(5);
			GraphicsItem->SetParentUniqueOption(5);
			
			GraphicsItem->SetMethodToGetIndexFromFile([LocalSettings]() { return LocalSettings->GetOverallScalabilityLevel(); });
			
			ParentOptions.Add(GraphicsItem);
			
			GraphicsQuality->AddSetting(GraphicsItem);
		}
		
		//----------------------------------------------------------------------------------
		{
			UGameSettingsItem* FrameRateLimitItem = NewObject<UGameSettingsItem>();
			FrameRateLimitItem->SetOptionName(FText::FromString("Frame Rate Limit"));
			FrameRateLimitItem->SetDescriptionRichText(FText::FromString("Select a desired framerate. Use this to fine tune performance on your device."));
			
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
			FrameRateLimitItem->SetIndexCurrentOption(LocalSettings->GetFrameRateLimit() == 0 ? 0 : LocalSettings->GetFrameRateLimit() / 30);
			
			GraphicsQuality->AddSetting(FrameRateLimitItem);
		}
		//----------------------------------------------------------------------------------

		 UGameSettingsItem* AutoSetQuality = NewObject<UGameSettingsItem>();
		 //----------------------------------------------------------------------------------
		 {
		 	AutoSetQuality->SetOptionName(FText::FromString("Auto-Set Quality"));
		 	AutoSetQuality->SetDescriptionRichText(FText::FromString("Automatically configure the graphics quality options based on a benchmark of the hardware."));
		
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
		 	LightingQualityItem->SetOptionName(FText::FromString("Lighting Quality"));
		 	LightingQualityItem->SetDescriptionRichText(FText::FromString("Lighting Quality affects the quality of lighting and shadows. Lower settings can improve performance."));
		
		 	LightingQualityItem->SetType(ESettingsType::Normal);
		 	
		 	LightingQualityItem->GetCurrentOptionValueDelegate().BindLambda( [LocalSettings,LightingQualityItem] ()
		 	{
		 		LocalSettings->SetGlobalIlluminationQuality(LightingQualityItem->GetTechnicalOption() );
		 	} );
		 	
		 	LightingQualityItem->SetTechnicalOption(GenericQualityOptions);
		 	LightingQualityItem->SetIndexCurrentOption(LocalSettings->GetGlobalIlluminationQuality());
		
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
		 	ShadowsItem->SetOptionName(FText::FromString("Shadows"));
		 	ShadowsItem->SetDescriptionRichText(FText::FromString("Shadows affect the quality of shadows cast by lights. Lower settings can improve performance."));
		
		 	ShadowsItem->SetType(ESettingsType::Normal);
		 	
		 	ShadowsItem->GetCurrentOptionValueDelegate().BindLambda( [LocalSettings,ShadowsItem] ()
		 	{
		 		LocalSettings->SetShadowQuality(ShadowsItem->GetTechnicalOption());
		 	} );
		 	
		 	ShadowsItem->SetTechnicalOption(GenericQualityOptions);
		 	ShadowsItem->SetIndexCurrentOption( LocalSettings->GetShadowQuality() );
		
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
		 	AntiAliasingItem->SetOptionName(FText::FromString("Anti-Aliasing"));
		 	AntiAliasingItem->SetDescriptionRichText(FText::FromString("Anti-Aliasing affects the smoothness of edges. Lower settings can improve performance."));
		
		 	AntiAliasingItem->SetType(ESettingsType::Normal);
		 	
		 	AntiAliasingItem->GetCurrentOptionValueDelegate().BindLambda( [LocalSettings,AntiAliasingItem] ()
		 	{
		 		LocalSettings->SetAntiAliasingQuality(AntiAliasingItem->GetTechnicalOption());
		 	} );
		 	
		 	AntiAliasingItem->SetTechnicalOption(GenericQualityOptions);
		 	AntiAliasingItem->SetIndexCurrentOption(LocalSettings->GetAntiAliasingQuality());
		
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
		 	ViewDistanceItem->SetOptionName(FText::FromString("View Distance"));
		 	ViewDistanceItem->SetDescriptionRichText(FText::FromString("View Distance affects the distance at which objects are drawn. Lower settings can improve performance."));
		
		 	ViewDistanceItem->SetType(ESettingsType::Normal);
		 	
		 	ViewDistanceItem->GetCurrentOptionValueDelegate().BindLambda( [LocalSettings,ViewDistanceItem] ()
		 	{
		 		LocalSettings->SetViewDistanceQuality(ViewDistanceItem->GetTechnicalOption());
		 	} );
		 	
		 	ViewDistanceItem->SetTechnicalOption(GenericQualityOptions);
		 	ViewDistanceItem->SetIndexCurrentOption(LocalSettings->GetViewDistanceQuality());
		
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
		 	TexturesItem->SetOptionName(FText::FromString("Textures"));
		 	TexturesItem->SetDescriptionRichText(FText::FromString("Texture quality determines the resolution of textures in game. Increasing this setting will make objects more detailed, but can reduce performance."));
		
		 	TexturesItem->SetType(ESettingsType::Normal);
		 	
		 	TexturesItem->GetCurrentOptionValueDelegate().BindLambda( [LocalSettings,TexturesItem] ()
		 	{
		 		LocalSettings->SetTextureQuality(TexturesItem->GetTechnicalOption());
		 	});
		 	
		 	TexturesItem->SetTechnicalOption(GenericQualityOptions);
		 	TexturesItem->SetIndexCurrentOption(LocalSettings->GetTextureQuality());
		
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
		 	EffectItem->SetOptionName(FText::FromString("Effects"));
		 	EffectItem->SetDescriptionRichText(FText::FromString("Effects quality affects the quality of particle effects. Lower settings can improve performance."));
		
		 	EffectItem->SetType(ESettingsType::Normal);
		 	
		 	EffectItem->GetCurrentOptionValueDelegate().BindLambda( [LocalSettings,EffectItem] ()
		 	{
		 		LocalSettings->SetVisualEffectQuality(EffectItem->GetTechnicalOption());
		 	} );
		 	
		 	EffectItem->SetTechnicalOption(GenericQualityOptions);
		 	EffectItem->SetIndexCurrentOption(LocalSettings->GetVisualEffectQuality());
		
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
		 	ReflectionsItem->SetOptionName(FText::FromString("Reflections"));
		 	ReflectionsItem->SetDescriptionRichText(FText::FromString("Reflection quality determines the resolution and accuracy of reflections.  Settings of 'High' and above use more accurate ray tracing methods to solve reflections, but can reduce performance."));
		
		 	ReflectionsItem->SetType(ESettingsType::Normal);
		 	
		 	ReflectionsItem->GetCurrentOptionValueDelegate().BindLambda( [LocalSettings,ReflectionsItem] ()
		 	{
		 		LocalSettings->SetReflectionQuality(ReflectionsItem->GetTechnicalOption());
		 	} );
		
		 	ReflectionsItem->SetTechnicalOption(GenericQualityOptions);
		 	ReflectionsItem->SetIndexCurrentOption(LocalSettings->GetReflectionQuality());
		
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
		 	PostProcessingItem->SetOptionName(FText::FromString("Post Processing"));
		 	PostProcessingItem->SetDescriptionRichText(FText::FromString("Post Processing effects include Motion Blur, Depth of Field and Bloom. Increasing this setting improves the quality of post process effects, but can reduce performance."));
		
		 	PostProcessingItem->SetType(ESettingsType::Normal);
		 	
		 	PostProcessingItem->GetCurrentOptionValueDelegate().BindLambda( [LocalSettings,PostProcessingItem] ()
		 	{
		 		LocalSettings->SetPostProcessingQuality(PostProcessingItem->GetTechnicalOption());
		 	} );
		 	
		 	PostProcessingItem->SetTechnicalOption(GenericQualityOptions);
		 	PostProcessingItem->SetIndexCurrentOption(LocalSettings->GetPostProcessingQuality());
		
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
		AdvancedGraphics->SetTitle(FText::FromString("Advanced Graphics"));
		Screen->AddSettingCollection(AdvancedGraphics);

		//----------------------------------------------------------------------------------
		{
			UGameSettingsItem* VerticalSyncItem = NewObject<UGameSettingsItem>();
			VerticalSyncItem->SetOptionName(FText::FromString("Vertical Sync"));
			VerticalSyncItem->SetDescriptionRichText(FText::FromString(
				"Vertical Sync synchronizes the game's framerate with the monitor's refresh rate. This can reduce screen tearing, but can also reduce performance."));

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
			VerticalSyncItem->SetMethodToGetIndexFromFile(	[LocalSettings]() { return LocalSettings->IsVSyncEnabled() ? 1 : 0; });
			
			AdvancedGraphics->AddSetting(VerticalSyncItem);
		}
		//----------------------------------------------------------------------------------
	}
	
	return Screen;
}