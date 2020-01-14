#pragma once

#include "UI/Widgets/CameraBordersWidget.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CameraHandler.generated.h"

class UCameraComponent;
class UWidgetComponent;

UCLASS()
class TBS_PROJECT_API ACameraHandler : public AActor
{
	GENERATED_BODY()
	
public:	
	ACameraHandler();
	void Tick(float DeltaTime) override;

protected:
	void BeginPlay() override;

private:
	void ProcessCameraState();
	void SetCameraLocation(const FVector& position);
	void ChangeState(const CameraMoveState& newState);

private:
	UPROPERTY()
	UCameraComponent* m_Camera = nullptr;

    UPROPERTY()
    UWidgetComponent* m_widget = nullptr;

	CameraMoveState m_CameraState = CameraMoveState::Idle;
};
