#include "TBS_ProjectGameModeBase.h"
#include "UI/Menus/MainMenu.h"
#include "Network/NetworkManager.h"
#include "Game/CameraHandler.h"
#include "Game/LevelTile.h"

ATBS_ProjectGameModeBase::ATBS_ProjectGameModeBase()
{
    DefaultPawnClass = nullptr;
    SpectatorClass = nullptr;
}

void ATBS_ProjectGameModeBase::BeginPlay()
{
    Super::BeginPlay();

    APlayerController* controller = GetWorld()->GetFirstPlayerController();
    if (controller)
    {
        FInputModeGameAndUI inputMode;
        inputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
        controller->SetInputMode(inputMode);
        
        controller->bShowMouseCursor = true;
        controller->bEnableMouseOverEvents = true;
        controller->bEnableClickEvents = true;
    }

    GetWorld()->SpawnActor<ACameraHandler>();
    GetWorld()->SpawnActor<ALevelTile>();
    //GetWorld()->SpawnActor<ANetworkManager>();
    //GetWorld()->SpawnActor<AMainMenu>();
}
