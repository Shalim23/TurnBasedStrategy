#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputHandler.generated.h"

UCLASS()
class TBS_PROJECT_API AInputHandler : public APawn
{
	GENERATED_BODY()

public:
	AInputHandler();
	void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void Tick(float DeltaTime) override;

protected:
	void BeginPlay() override;

private:
	void ZoomIn();
	void ZoomOut();

};
