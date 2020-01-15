#pragma once

#include "Custom/Events/EventsHandler.h"
#include "UI/Widgets/CameraBordersWidget.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CameraHandler.generated.h"

class UCameraComponent;
class UWidgetComponent;
struct EventData;

UCLASS()
class TBS_PROJECT_API ACameraHandler : public AActor
{
	GENERATED_BODY()
	
public:	
	ACameraHandler();
	void Tick(float DeltaTime) override;

protected:
	void BeginPlay() override;
	void EndPlay(const EEndPlayReason::Type reason) override;

private:
	void ProcessCameraState();
	void SetCameraLocation(const FVector& position);
	void ChangeState(const CameraMoveState& newState);

	void SubscribeOnEvents();

	void OnCameraZoom(const EventData& eventData);

private:
	UPROPERTY()
	UCameraComponent* m_Camera = nullptr;

    UPROPERTY()
    UWidgetComponent* m_widget = nullptr;

	CameraMoveState m_CameraState = CameraMoveState::Idle;
	EventsHandler m_EventsHandler;
};
