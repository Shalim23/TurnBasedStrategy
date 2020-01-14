#include "CameraHandler.h"
#include "Camera/CameraComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/WidgetComponent.h"


//#TODO add zoom in/out
ACameraHandler::ACameraHandler()
{
 	PrimaryActorTick.bCanEverTick = true;
	m_Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

    m_widget = CreateDefaultSubobject<UWidgetComponent>(TEXT("CameraBordersWidget"));
    ConstructorHelpers::FClassFinder<UUserWidget> widget(TEXT("/Game/CameraBordersWidget"));
    if (widget.Succeeded())
    {
        m_widget->SetWidgetClass(widget.Class);
    }

    SetCameraLocation(FVector(0.0f, 0.0f, 300.0f));
    m_Camera->SetWorldRotation(FQuat(FRotator(-90.0f, 0.0f, 0.0f)), false, 0, ETeleportType::None);
}

void ACameraHandler::BeginPlay()
{
	Super::BeginPlay();

    if (UCameraBordersWidget* widget = Cast<UCameraBordersWidget>(m_widget->GetUserWidgetObject()))
    {
        widget->SetChangeStateCallback([this](const CameraMoveState& newState) { ChangeState(newState); });
    }

    APlayerController* controller = GetWorld()->GetFirstPlayerController();
    if (controller)
    {
        controller->SetViewTarget(this);
    }
}

void ACameraHandler::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    ProcessCameraState();
}

void ACameraHandler::ProcessCameraState()
{
    const float OffsetToAdd = 3.0f;
    switch (m_CameraState)
    {
        case CameraMoveState::Top:
        {
            SetCameraLocation(m_Camera->GetComponentLocation() + FVector(OffsetToAdd, 0.0f, 0.0f));
            break;
        }
        case CameraMoveState::Bottom:
        {
            SetCameraLocation(m_Camera->GetComponentLocation() + FVector(-OffsetToAdd, 0.0f, 0.0f));
            break;
        }
        case CameraMoveState::Left:
        {
            SetCameraLocation(m_Camera->GetComponentLocation() + FVector(0.0f, -OffsetToAdd, 0.0f));
            break;
        }
        case CameraMoveState::Right:
        {
            SetCameraLocation(m_Camera->GetComponentLocation() + FVector(0.0f, OffsetToAdd, 0.0f));
            break;
        }
        case CameraMoveState::TopLeft:
        {
            SetCameraLocation(m_Camera->GetComponentLocation() + FVector(OffsetToAdd, -OffsetToAdd, 0.0f));
            break;
        }
        case CameraMoveState::TopRight:
        {
            SetCameraLocation(m_Camera->GetComponentLocation() + FVector(OffsetToAdd, OffsetToAdd, 0.0f));
            break;
        }
        case CameraMoveState::BottomLeft:
        {
            SetCameraLocation(m_Camera->GetComponentLocation() + FVector(-OffsetToAdd, -OffsetToAdd, 0.0f));
            break;
        }
        case CameraMoveState::BottomRight:
        {
            SetCameraLocation(m_Camera->GetComponentLocation() + FVector(-OffsetToAdd, OffsetToAdd, 0.0f));
            break;
        }
    }
}

void ACameraHandler::SetCameraLocation(const FVector& position)
{
    m_Camera->SetWorldLocation(position, false, 0, ETeleportType::None);
}

void ACameraHandler::ChangeState(const CameraMoveState& newState)
{
    m_CameraState = newState;
}

