#pragma once
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

    void OnNoConnection();
    void OnConnected();
    void OnGameFound();
    void OnWaitingForPlayers();
    void OnNotReady();
    void OnConnectionLost();

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
    void SetMessageText(const char* text);


    UFUNCTION()
        void ReturnToMainMenu();

    UFUNCTION()
        void OnReadyButtonPressed();

private:
    bool m_IsConnected = false;
    bool m_IsNoConnection = false;

};
