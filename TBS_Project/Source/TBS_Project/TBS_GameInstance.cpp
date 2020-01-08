// Fill out your copyright notice in the Description page of Project Settings.


#include "TBS_GameInstance.h"
#include "Custom/Singleton/SingletonManager.h"

UTBS_GameInstance::UTBS_GameInstance(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    SingletonManager::GetInstance().Init();
}

void UTBS_GameInstance::Shutdown()
{
    Super::Shutdown();

    SingletonManager::GetInstance().Shutdown();
}