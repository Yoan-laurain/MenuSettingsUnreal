#pragma once

#include "Subsystems/WorldSubsystem.h"
#include "AudioMixEffectSubSystem.generated.h"

class FSubsystemCollectionBase;
class UObject;
class USoundControlBus;
class USoundControlBusMix;
class USoundEffectSubmixPreset;
class USoundSubmix;
class UWorld;

UCLASS()
class MENUSETTINGS_API UAudioMixEffectSubSystem final : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	// USubsystem implementation Begin
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	// USubsystem implementation End

	/** Called once all UWorldSubsystems have been initialized */
	virtual void PostInitialize() override;

	/** Called when world is ready to start gameplay before the game mode transitions to the correct state and call BeginPlay on all actors */
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	
protected:
	
	UPROPERTY(Transient)
	TObjectPtr<USoundControlBusMix> DefaultBaseMix = nullptr;
	
	UPROPERTY(Transient)
	TObjectPtr<USoundControlBusMix> UserMix = nullptr;
	
	UPROPERTY(Transient)
	TObjectPtr<USoundControlBus> OverallControlBus = nullptr;
	
	UPROPERTY(Transient)
	TObjectPtr<USoundControlBus> MusicControlBus = nullptr;
	
	UPROPERTY(Transient)
	TObjectPtr<USoundControlBus> SoundFXControlBus = nullptr;
	
	UPROPERTY(Transient)
	TObjectPtr<USoundControlBus> DialogueControlBus = nullptr;
};