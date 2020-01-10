#include "MainMenuWidget.h"
#include "Components/Button.h"
#include "Custom/Events/EventDispatcher.h"
#include "UI/Menus/FindGameMenu.h"
#include "UI/Menus/GameRulesMenu.h"

UMainMenuWidget::UMainMenuWidget(const FObjectInitializer& objectInitializer)
    : Super(objectInitializer)
{}

void UMainMenuWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (StartGameButton)
    {
        StartGameButton->OnPressed.AddDynamic(this, &UMainMenuWidget::FindGame);
    }

    if (GameRulesButton)
    {
        GameRulesButton->OnPressed.AddDynamic(this, &UMainMenuWidget::GoToGameRules);
    }

    if (ExitGameButton)
    {
        ExitGameButton->OnPressed.AddDynamic(this, &UMainMenuWidget::ExitGame);
    }

    AddToViewport();
}

void UMainMenuWidget::GoToGameRules()
{
    if (BaseGameEvent* leaveFromMainMenuEvent =
        EventDispatcher::GetInstance().GetEvent(GameplayEventType::LeaveFromMainMenu))
    {
        leaveFromMainMenuEvent->Broadcast(LeaveFromMainMenuEventData());

        RemoveFromParent();
        GetWorld()->SpawnActor<AGameRulesMenu>();
    }
}

void UMainMenuWidget::ExitGame()
{
    FGenericPlatformMisc::RequestExit(false);
}

void UMainMenuWidget::FindGame()
{
    EventDispatcher& eventDispatcherInstance = EventDispatcher::GetInstance();
    BaseGameEvent* findGameEvent = eventDispatcherInstance.GetEvent(GameplayEventType::FindGame);
    BaseGameEvent* leaveFromMainMenuEvent = eventDispatcherInstance.GetEvent(GameplayEventType::LeaveFromMainMenu);

    if (findGameEvent && leaveFromMainMenuEvent)
    {
        RemoveFromParent();
        GetWorld()->SpawnActor<AFindGameMenu>();

        findGameEvent->Broadcast(FindGameEventData());
        leaveFromMainMenuEvent->Broadcast(LeaveFromMainMenuEventData());
    }
}