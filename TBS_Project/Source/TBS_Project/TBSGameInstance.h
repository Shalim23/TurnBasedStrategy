#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "TBSGameInstance.generated.h"


UCLASS()
class TBS_PROJECT_API UTBSGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
    UTBSGameInstance(const FObjectInitializer& ObjectInitializer);
    void Shutdown() override;
};
