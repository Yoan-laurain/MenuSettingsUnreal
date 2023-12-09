#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CustomGameInstance.generated.h"

UCLASS()
class MENUSETTINGS_API UCustomGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	virtual void Shutdown() override;
};