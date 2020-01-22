#include "CameraHandler.h"
#include "Camera/CameraComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/WidgetComponent.h"
#include "Custom/Events/Events.h"
#include "Custom/Utils/PrintScreenHelper.h"

ACameraHandler::ACameraHandler()
{
    PrimaryActorTick.bCanEverTick = true;
    m_Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

//     m_widget = CreateDefaultSubobject<UWidgetComponent>(TEXT("CameraBordersWidget"));
//     ConstructorHelpers::FClassFinder<UUserWidget> widget(TEXT("/Game/CameraBordersWidget"));
//     if (widget.Succeeded())
//     {
//         m_widget->SetWidgetClass(widget.Class);
//     }

    //SetCameraLocation(FVector(0.0f, 0.0f, 300.0f));

    RootComponent = m_Camera;
    RootComponent->SetWorldRotation(FRotator(-20.0f, 0.0f, 0.0f), false, 0, ETeleportType::None);

    //SubscribeOnEvents();
}

void ACameraHandler::BeginPlay()
{
	Super::BeginPlay();

//     if (UCameraBordersWidget* widget = Cast<UCameraBordersWidget>(m_widget->GetUserWidgetObject()))
//     {
//         widget->SetChangeStateCallback([this](const CameraMoveState& newState) { ChangeState(newState); });
//     }

    APlayerController* controller = GetWorld()->GetFirstPlayerController();
    if (controller)
    {
        controller->SetViewTarget(this);
    }
}

void ACameraHandler::EndPlay(const EEndPlayReason::Type reason)
{
    Super::EndPlay(reason);
}

void ACameraHandler::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    //ProcessCameraState();
}

void ACameraHandler::Activate()
{
    APlayerController* controller = GetWorld()->GetFirstPlayerController();
    if (controller)
    {
        controller->SetViewTarget(this);
    }
}

void ACameraHandler::ProcessCameraState()
{
    /*const float OffsetToAdd = 3.0f;
    switch (m_CameraState)
    {
        case CameraMoveState::Right:
        {
            SetCameraLocation(m_Camera->GetComponentLocation() + FVector(OffsetToAdd, 0.0f, 0.0f));
            break;
        }
        case CameraMoveState::Left:
        {
            SetCameraLocation(m_Camera->GetComponentLocation() + FVector(-OffsetToAdd, 0.0f, 0.0f));
            break;
        }
        case CameraMoveState::Top:
        {
            SetCameraLocation(m_Camera->GetComponentLocation() + FVector(0.0f, -OffsetToAdd, 0.0f));
            break;
        }
        case CameraMoveState::Bottom:
        {
            SetCameraLocation(m_Camera->GetComponentLocation() + FVector(0.0f, OffsetToAdd, 0.0f));
            break;
        }
        case CameraMoveState::TopLeft:
        {
            SetCameraLocation(m_Camera->GetComponentLocation() + FVector(-OffsetToAdd, -OffsetToAdd, 0.0f));
            break;
        }
        case CameraMoveState::TopRight:
        {
            SetCameraLocation(m_Camera->GetComponentLocation() + FVector(OffsetToAdd, -OffsetToAdd, 0.0f));
            break;
        }
        case CameraMoveState::BottomLeft:
        {
            SetCameraLocation(m_Camera->GetComponentLocation() + FVector(-OffsetToAdd, OffsetToAdd, 0.0f));
            break;
        }
        case CameraMoveState::BottomRight:
        {
            SetCameraLocation(m_Camera->GetComponentLocation() + FVector(OffsetToAdd, OffsetToAdd, 0.0f));
            break;
        }
    }*/
}

void ACameraHandler::UpdateCamera(const FVector& direction, const FVector& location)
{
    FVector constDistance(-800.0f, 0.0f, 500.0f);

    if (!direction.IsNearlyZero(0.1f))
    {
        FRotator newRotation = direction.Rotation();
        newRotation.Pitch = -20.0f;
        newRotation.Roll = 0.0f;

        constDistance = constDistance.RotateAngleAxis(newRotation.Yaw, FVector(0.0f, 0.0f, 1.0f));
        RootComponent->SetRelativeRotation(newRotation, false, 0, ETeleportType::None);
    }

    FVector newPosition = constDistance + location;
    m_Camera->SetWorldLocation(newPosition, false, 0, ETeleportType::None);

    //PrintOnScreenHelper::PrintOnScreenFVector(m_Camera->GetComponentLocation(), FColor::Green);
    //PrintOnScreenHelper::PrintOnScreenFVector(m_Camera->GetComponentRotation(), FColor::Yellow);
}

void ACameraHandler::ChangeState(const CameraMoveState& newState)
{
    m_CameraState = newState;
}

void ACameraHandler::SubscribeOnEvents()
{
    m_EventsHandler.subscribe({
        {GameplayEventType::CameraZoom, [this](const EventData& eventData) { OnCameraZoom(eventData); }},
        });
}

void ACameraHandler::OnCameraZoom(const EventData& eventData)
{
    if (eventData.eventType == GameplayEventType::CameraZoom)
    {
        //#TODO add clamp
        
        const CameraZoomEventData& cameraZoomEvent =
            static_cast<const CameraZoomEventData&>(eventData);

        //SetCameraLocation(m_Camera->GetComponentLocation() + FVector(0.0f, 0.0f, cameraZoomEvent.scaleValue));

        PrintOnScreenHelper::PrintOnScreenFVector(m_Camera->GetComponentLocation());
    }
}

