#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "RollingBall.generated.h"

class UStaticMeshComponent;
class ACameraHandler;

UCLASS()
class TBS_PROJECT_API ARollingBall : public APawn
{
	GENERATED_BODY()

public:
	ARollingBall();
    void Tick(float DeltaTime) override;

    void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
    void SetCamera(ACameraHandler* camera);

protected:
	void BeginPlay() override;

private:
    void MoveForwardBackward(float value);
    void MoveLeftRight(float value);

private:
    UPROPERTY()
    ACameraHandler* m_Camera = nullptr;

    UPROPERTY()
    UStaticMeshComponent* m_Mesh = nullptr;

};
