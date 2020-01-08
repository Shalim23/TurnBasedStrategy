// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "TBS_GameInstance.generated.h"

/**
 * 
 */
UCLASS()
class TBS_PROJECT_API UTBS_GameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
    UTBS_GameInstance(const FObjectInitializer& ObjectInitializer);
    void Shutdown() override;
};
