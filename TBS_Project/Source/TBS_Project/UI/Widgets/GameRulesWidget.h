#pragma once

#include <vector>
#include <memory>
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameRulesWidget.generated.h"

class UButton;
class UHorizontalBox;
class UTextBlock;
class UScrollBox;
struct GameRulesCategoryData;
struct GameRulesData;

USTRUCT()
struct FRulesCategoryChildren
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY()
    TArray<UWidget*> Children;
};

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
        UScrollBox* ButtonsScrollBox;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        UTextBlock* HeaderText;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        UScrollBox* ScrollBoxWithInstructions;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        UHorizontalBox* RulesCategoryButtons;

private:
    void NativeConstruct() override;
    void InitButtons();

    void InitRulesCategoryButtons(const std::vector<GameRulesCategoryData>& data);

    UFUNCTION()
        void ReturnToMainMenu();

    UFUNCTION()
        void OnGameRulesButtonPressed();

    UFUNCTION()
        void OnRulesCategoryButtonPressed();

    void SetInstructionsScrollBoxData(const GameRulesData& currentData);
    void SetInstructionsButtons(const GameRulesCategoryData& categoryData);

private:
    TArray<FRulesCategoryChildren> m_ButtonsScrollBoxChildren;

    size_t m_CurrentTabIndex = 0;
};