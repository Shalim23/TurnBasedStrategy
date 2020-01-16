#include "InputHandler.h"
#include "Custom/Utils/PrintScreenHelper.h"
#include "Custom/Utils/Macros.h"
#include "Custom/Events/EventDispatcher.h"

AInputHandler::AInputHandler()
{
    INIT_ONCE(
        PrimaryActorTick.bCanEverTick = true;
        )
}

void AInputHandler::BeginPlay()
{
	Super::BeginPlay();
	
}

void AInputHandler::ZoomIn()
{
    if (BaseGameEvent* cameraZoomEvent =
        EventDispatcher::GetInstance().GetEvent(GameplayEventType::CameraZoom))
    {
        cameraZoomEvent->Broadcast(CameraZoomEventData(-20.0f));
    }
}

void AInputHandler::ZoomOut()
{
    if (BaseGameEvent* cameraZoomEvent =
        EventDispatcher::GetInstance().GetEvent(GameplayEventType::CameraZoom))
    {
        cameraZoomEvent->Broadcast(CameraZoomEventData(20.0f));
    }
}

void AInputHandler::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AInputHandler::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("ZoomIn", IE_Pressed, this, &AInputHandler::ZoomIn);
	PlayerInputComponent->BindAction("ZoomOut", IE_Pressed, this, &AInputHandler::ZoomOut);
}

