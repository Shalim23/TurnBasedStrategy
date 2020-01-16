#include "TBS_ProjectGameModeBase.h"
#include "UI/Menus/MainMenu.h"
#include "Network/NetworkManager.h"
#include "Game/CameraHandler.h"
#include "Game/InputHandler.h"
#include "Game/Tutorial/TutorialLevelHandler.h"

ATBS_ProjectGameModeBase::ATBS_ProjectGameModeBase()
{
    DefaultPawnClass = nullptr;
    SpectatorClass = nullptr;
}

void ATBS_ProjectGameModeBase::BeginPlay()
{
    Super::BeginPlay();

    if (UWorld* world = GetWorld())
    {
        if (APlayerController* controller = world->GetFirstPlayerController())
        {
            FInputModeGameAndUI inputMode;
            inputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);

            controller->SetInputMode(inputMode);
            controller->bShowMouseCursor = true;
            controller->bEnableMouseOverEvents = true;
            controller->bEnableClickEvents = true;

            //controller->Possess(world->SpawnActor<AInputHandler>());

            //world->SpawnActor<ATutorialLevelHandler>();

            //world->SpawnActor<ACameraHandler>();
            world->SpawnActor<ANetworkManager>();
            world->SpawnActor<AMainMenu>();
        }
    }
}
