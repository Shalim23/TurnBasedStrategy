#include "GameRulesWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Custom/Events/EventDispatcher.h"
#include "UI/Menus/MainMenu.h"

UGameRulesWidget::UGameRulesWidget(const FObjectInitializer& objectInitializer)
    : Super(objectInitializer)
{}

void UGameRulesWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (ToMainMenuButton)
    {
        ToMainMenuButton->OnPressed.AddDynamic(this, &UGameRulesWidget::ReturnToMainMenu);
    }

    AddToViewport();
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
