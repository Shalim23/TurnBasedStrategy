#pragma once

#include "CoreMinimal.h"
#include "Engine/GameViewportClient.h"
#include "TBSGameViewportClient.generated.h"

UCLASS()
class TBS_PROJECT_API UTBSGameViewportClient : public UGameViewportClient
{
	GENERATED_BODY()
	
public:
    void ReceivedFocus(FViewport* viewport) override;
};
