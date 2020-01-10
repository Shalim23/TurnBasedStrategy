#include "GameRulesWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/ScrollBox.h"
#include "Components/SizeBox.h"
#include "Custom/Events/EventDispatcher.h"
#include "UI/Menus/MainMenu.h"
#include "Custom/JsonDataHelper/JsonDataHelper.h"

UGameRulesWidget::UGameRulesWidget(const FObjectInitializer& objectInitializer)
    : Super(objectInitializer)
{}

void UGameRulesWidget::NativeConstruct()
{
    Super::NativeConstruct();

    InitButtons();

    AddToViewport();
}

void UGameRulesWidget::InitRulesCategoryButtons(const std::vector<GameRulesCategoryData>& data)
{
    constexpr float ButtonRightPadding = 50.0f;

    for (const GameRulesCategoryData& gameRulesCategories : data)
    {
        UTextBlock* text = NewObject<UTextBlock>();
        text->ColorAndOpacity = FLinearColor(0.0f, 0.0f, 0.0f);
        text->SetText(FText::FromString(gameRulesCategories.header));

        UButton* button = NewObject<UButton>();
        button->OnPressed.AddDynamic(this, &UGameRulesWidget::OnRulesCategoryButtonPressed);
        button->AddChild(text);

        RulesCategoryButtons->AddChild(button);
    }

    TArray<UPanelSlot*> slots = RulesCategoryButtons->GetSlots();
    for (UPanelSlot* slot : slots)
    {
        if (slot && slot->IsA<UHorizontalBoxSlot>())
        {
            UHorizontalBoxSlot* horizontalBoxSlot = static_cast<UHorizontalBoxSlot*>(slot);
            horizontalBoxSlot->SetPadding(FMargin(0.0f, 0.0f, ButtonRightPadding, 0.0f));
            horizontalBoxSlot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
        }
    }
}

void UGameRulesWidget::InitButtons()
{
    const std::vector<GameRulesCategoryData>& data =
        JsonDataHelper::GetInstance().GetGameRulesDataHelper().GetData();

    if (ToMainMenuButton)
    {
        ToMainMenuButton->OnPressed.AddDynamic(this, &UGameRulesWidget::ReturnToMainMenu);
    }

    if (RulesCategoryButtons)
    {
        InitRulesCategoryButtons(data);
    }

    if (ButtonsScrollBox)
    {
        const GameRulesCategoryData& categoryData = data[m_CurrentTabIndex];
        m_ButtonsScrollBoxChildren.SetNum(categoryData.gameRulesData.size());

        SetInstructionsButtons(categoryData);
        SetInstructionsScrollBoxData(categoryData.gameRulesData.front());
    }
}

void UGameRulesWidget::SetInstructionsScrollBoxData(const GameRulesData& currentData)
{
    if (HeaderText && ScrollBoxWithInstructions)
    {
        HeaderText->SetText(FText::FromString(currentData.header));
        UWidget* text = ScrollBoxWithInstructions->GetChildAt(0); //ScrollBoxWithInstructions has only one child
        if (text && text->IsA<UTextBlock>())
        {
            UTextBlock* textBlock = static_cast<UTextBlock*>(text);
            textBlock->SetText(FText::FromString(currentData.contents));
            ScrollBoxWithInstructions->ScrollToStart();
        }
    }
}

void UGameRulesWidget::SetInstructionsButtons(const GameRulesCategoryData& categoryData)
{
    constexpr float SizeBoxHeight = 100.0f;
    FRulesCategoryChildren& categoryChildren = m_ButtonsScrollBoxChildren[m_CurrentTabIndex];
    if (categoryChildren.Children.Num() == 0)
    {
        for (const GameRulesData& rulesData : categoryData.gameRulesData)
        {
            UTextBlock* text = NewObject<UTextBlock>();
            text->ColorAndOpacity = FLinearColor(0.0f, 0.0f, 0.0f);
            text->SetText(FText::FromString(rulesData.header));

            UButton* button = NewObject<UButton>();
            button->OnPressed.AddDynamic(this, &UGameRulesWidget::OnGameRulesButtonPressed);
            button->AddChild(text);

            USizeBox* sizeBox = NewObject<USizeBox>();
            sizeBox->SetHeightOverride(SizeBoxHeight);
            sizeBox->bOverride_HeightOverride = true;
            sizeBox->SetVisibility(ESlateVisibility::Visible);
            sizeBox->AddChild(button);

            categoryChildren.Children.Emplace(sizeBox);
        }
    }

    TArray<UWidget*> buttons = ButtonsScrollBox->GetAllChildren();
    if (buttons.Num() > 0)
    {
        for (UWidget* button : buttons)
        {
            ButtonsScrollBox->RemoveChild(button);
        }
    }

    for (UWidget* child : categoryChildren.Children)
    {
        ButtonsScrollBox->AddChild(child);
    }

    SetInstructionsScrollBoxData(categoryData.gameRulesData.front());
}

void UGameRulesWidget::OnRulesCategoryButtonPressed()
{
    TArray<UWidget*> buttons = RulesCategoryButtons->GetAllChildren();
    for (size_t i = 0; i < buttons.Num(); ++i)
    {
        UWidget* widget = buttons[i];
        if (!widget || !widget->IsA<UButton>())
        {
            continue;
        }

        UButton* button = static_cast<UButton*>(widget);
        if (button->IsPressed())
        {
            m_CurrentTabIndex = i;

            const std::vector<GameRulesCategoryData>& data =
                JsonDataHelper::GetInstance().GetGameRulesDataHelper().GetData();

            SetInstructionsButtons(data[m_CurrentTabIndex]);
            return;
        }
    }
}

void UGameRulesWidget::OnGameRulesButtonPressed()
{
    TArray<UWidget*> buttons = ButtonsScrollBox->GetAllChildren();
    for (size_t i = 0; i < buttons.Num(); ++i)
    {
        UWidget* widget = buttons[i];
        if (!widget || !widget->IsA<USizeBox>())
        {
            continue;
        }

        USizeBox* sizeBox = static_cast<USizeBox*>(widget);
        UWidget* sizeBoxChild = sizeBox->GetChildAt(0);//sizeBox has only one child
        if (sizeBoxChild && sizeBoxChild->IsA<UButton>())
        {
            UButton* button = static_cast<UButton*>(sizeBoxChild);
            if (button->IsHovered())
            {
                const std::vector<GameRulesCategoryData>& data =
                    JsonDataHelper::GetInstance().GetGameRulesDataHelper().GetData();

                SetInstructionsScrollBoxData(data[m_CurrentTabIndex].gameRulesData[i]);

                return;
            }
        }
    }
}

void UGameRulesWidget::ReturnToMainMenu()
{
    if (BaseGameEvent* returnToMainMenuEvent =
        EventDispatcher::GetInstance().GetEvent(GameplayEventType::ReturnToMainMenu))
    {
        returnToMainMenuEvent->Broadcast(ReturnToMainMenuEventData());

        RemoveFromParent();
        GetWorld()->SpawnActor<AMainMenu>();
    }
}

