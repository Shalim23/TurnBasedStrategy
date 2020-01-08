#pragma once

#include <functional>
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FindGameWidget.generated.h"

class UButton;
class UTextBlock;

UCLASS()
class TBS_PROJECT_API UFindGameWidget : public UUserWidget
{
	GENERATED_BODY()
public:
    UFindGameWidget(const FObjectInitializer& objectInitializer);

    void SetMessageText(const char* text);
    void OnGameFound();
    void OnWaitingForPlayers();
    void OnNotReady();
    void OnConnectionLost();

    void SetOnConstructedCallback(std::function<void()> callback);

protected:
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        UButton* ToMainMenuButton;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        UButton* ReadyButton;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        UTextBlock* MessageText;

private:
    void NativeConstruct() override;
    void DeactivateReadyButton();


    UFUNCTION()
        void ReturnToMainMenu();

    UFUNCTION()
        void OnReadyButtonPressed();

private:
    std::function<void()> m_OnConstructedCallback;
};
