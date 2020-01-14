#pragma once
#include <functional>

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CameraBordersWidget.generated.h"

class UButton;

enum class CameraMoveState
{
    Top,
    Bottom,
    Left,
    Right,
    TopLeft,
    TopRight,
    BottomLeft,
    BottomRight,
    Idle
};

UCLASS()
class TBS_PROJECT_API UCameraBordersWidget : public UUserWidget
{
	GENERATED_BODY()
public:
    UCameraBordersWidget(const FObjectInitializer& objectInitializer);
    void SetChangeStateCallback(const std::function<void(const CameraMoveState&)>& callback);

protected:
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    UButton* Top;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    UButton* Bottom;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    UButton* TopLeft;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    UButton* Left;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    UButton* Right;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    UButton* TopRight;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    UButton* BottomRight;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    UButton* BottomLeft;

private:
    void NativeConstruct() override;

    UFUNCTION()
    void OnStateReset();
    UFUNCTION()
    void OnTop();
    UFUNCTION()
    void OnBottom();
    UFUNCTION()
    void OnTopLeft();
    UFUNCTION()
    void OnLeft();
    UFUNCTION()
    void OnRight();
    UFUNCTION()
    void OnTopRight();
    UFUNCTION()
    void OnBottomRight();
    UFUNCTION()
    void OnBottomLeft();

private:
    std::function<void(const CameraMoveState&)> m_ChangeStateCallback;
};
