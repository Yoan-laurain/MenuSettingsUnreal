#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameGM.generated.h"

UCLASS()
class MENUSETTINGS_API AGameGM : public AGameModeBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
};