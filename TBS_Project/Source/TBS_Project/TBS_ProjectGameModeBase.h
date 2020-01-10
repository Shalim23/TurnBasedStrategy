#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TBS_ProjectGameModeBase.generated.h"

UCLASS()
class TBS_PROJECT_API ATBS_ProjectGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
protected:
    void BeginPlay() override;
};
