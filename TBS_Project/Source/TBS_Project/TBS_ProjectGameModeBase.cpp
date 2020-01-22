#include "TBS_ProjectGameModeBase.h"
#include "UI/Menus/MainMenu.h"
#include "Network/NetworkManager.h"
#include "Game/CameraHandler.h"
#include "Game/InputHandler.h"
#include "Game/Tutorial/TutorialLevelHandler.h"
#include "Game/RollingBall.h"

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

            //ARollingBall* ball = world->SpawnActor<ARollingBall>(FVector(400.0f, 400.0f, 100.0f), FRotator(0.0f, 0.0f, 0.0f));
            //controller->Possess(ball);

            world->SpawnActor<ATutorialLevelHandler>();

            //ball->SetCamera(world->SpawnActor<ACameraHandler>());
            //world->SpawnActor<ANetworkManager>();
            //world->SpawnActor<AMainMenu>();
        }
    }
}
