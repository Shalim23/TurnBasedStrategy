// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.


#include "TBS_ProjectGameModeBase.h"

void ATBS_ProjectGameModeBase::BeginPlay()
{
    Super::BeginPlay();

    APlayerController* controller = GetWorld()->GetFirstPlayerController();
    if (controller)
    {
        controller->SetInputMode(FInputModeUIOnly());
        controller->bShowMouseCursor = true;
    }
}