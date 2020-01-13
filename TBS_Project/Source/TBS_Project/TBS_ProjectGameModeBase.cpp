#include "TBS_ProjectGameModeBase.h"
#include "UI/Menus/MainMenu.h"
#include "Network/NetworkManager.h"
#include "Game/CameraHandler.h"

ATBS_ProjectGameModeBase::ATBS_ProjectGameModeBase()
{
    DefaultPawnClass = nullptr;
}

void ATBS_ProjectGameModeBase::BeginPlay()
{
    Super::BeginPlay();

    APlayerController* controller = GetWorld()->GetFirstPlayerController();
    if (controller)
    {
        //controller->SetInputMode(FInputModeUIOnly());
        //controller->bShowMouseCursor = true;
    }

    GetWorld()->SpawnActor<ACameraHandler>();
    //GetWorld()->SpawnActor<ANetworkManager>();
}
