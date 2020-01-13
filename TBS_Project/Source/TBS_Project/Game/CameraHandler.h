#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CameraHandler.generated.h"

class UCameraComponent;

UCLASS()
class TBS_PROJECT_API ACameraHandler : public AActor
{
	GENERATED_BODY()
	
public:	
	ACameraHandler();

protected:
	void BeginPlay() override;

public:	
	void Tick(float DeltaTime) override;

private:
	UCameraComponent* m_Camera = nullptr;
};
