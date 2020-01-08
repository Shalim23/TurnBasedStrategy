#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameRulesWidget.generated.h"

class UButton;
class UTextBlock;

UCLASS()
class TBS_PROJECT_API UGameRulesWidget : public UUserWidget
{
	GENERATED_BODY()

public:
    UGameRulesWidget(const FObjectInitializer& objectInitializer);

protected:
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    UButton* ToMainMenuButton;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    UTextBlock* HeaderText;

private:
    void NativeConstruct() override;

    UFUNCTION()
    void ReturnToMainMenu();

};
