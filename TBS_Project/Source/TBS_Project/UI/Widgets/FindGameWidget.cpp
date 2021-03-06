#include "FindGameWidget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

#include "Custom/Events/EventDispatcher.h"
#include "UI/Menus/MainMenu.h"

UFindGameWidget::UFindGameWidget(const FObjectInitializer& objectInitializer)
    : Super(objectInitializer)
{}

void UFindGameWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (ToMainMenuButton)
    {
        ToMainMenuButton->OnPressed.AddDynamic(this, &UFindGameWidget::ReturnToMainMenu);
    }

    if (ReadyButton)
    {
        ReadyButton->OnPressed.AddDynamic(this, &UFindGameWidget::OnReadyButtonPressed);

        DeactivateReadyButton();
    }

    if (!m_IsConnected && !m_IsNoConnection)
    {
        SetMessageText("Finding game...");
    }

    AddToViewport();
}

void UFindGameWidget::SetMessageText(const char* text)
{
    if (MessageText)
    {
        MessageText->SetText(FText::FromString(text));
    }
}

void UFindGameWidget::OnNoConnection()
{
    m_IsNoConnection = true;
    SetMessageText("Server is down...");
}

void UFindGameWidget::OnConnected()
{
    m_IsConnected = true;
    SetMessageText("Connected... Waiting for players...");
}

void UFindGameWidget::OnGameFound()
{
    SetMessageText("Game is found...");

    if (ReadyButton)
    {
        ReadyButton->SetIsEnabled(true);
        ReadyButton->SetVisibility(ESlateVisibility::Visible);
    }
}

void UFindGameWidget::OnWaitingForPlayers()
{
    SetMessageText("Waiting for players...");
    DeactivateReadyButton();
}

void UFindGameWidget::OnNotReady()
{
    SetMessageText("You are not ready...");
    DeactivateReadyButton();
}

void UFindGameWidget::OnConnectionLost()
{
    SetMessageText("Connection lost...");
    DeactivateReadyButton();
}

void UFindGameWidget::ReturnToMainMenu()
{
    if (BaseGameEvent* returnToMainMenuEvent =
        EventDispatcher::GetInstance().GetEvent(GameplayEventType::ReturnToMainMenu))
    {
        returnToMainMenuEvent->Broadcast(ReturnToMainMenuEventData());

        RemoveFromParent();
        GetWorld()->SpawnActor<AMainMenu>();
    }
}

void UFindGameWidget::OnReadyButtonPressed()
{
    if (BaseGameEvent* readyForGameEvent =
        EventDispatcher::GetInstance().GetEvent(GameplayEventType::ReadyForGame))
    {
        DeactivateReadyButton();
        SetMessageText("Ready... Waiting for players...");

        readyForGameEvent->Broadcast(ReadyForGameEventData());
    }
}

void UFindGameWidget::DeactivateReadyButton()
{
    if (ReadyButton)
    {
        ReadyButton->SetIsEnabled(false);
        ReadyButton->SetVisibility(ESlateVisibility::Hidden);
    }
}

