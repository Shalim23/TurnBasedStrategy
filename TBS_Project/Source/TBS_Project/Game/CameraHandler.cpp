#include "CameraHandler.h"
#include "Camera/CameraComponent.h"

ACameraHandler::ACameraHandler()
{
 	PrimaryActorTick.bCanEverTick = true;
	m_Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
}

void ACameraHandler::BeginPlay()
{
	Super::BeginPlay();

    APlayerController* controller = GetWorld()->GetFirstPlayerController();
    if (controller)
    {
        m_Camera->SetWorldLocation(FVector(0.0f, 0.0f, 300.0f), false, 0, ETeleportType::None);
        m_Camera->SetWorldRotation(FQuat(FRotator(-90.0f, 0.0f, 0.0f)), false, 0, ETeleportType::None);
        controller->SetViewTarget(this);
    }
}

void ACameraHandler::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

