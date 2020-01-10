#include "FindGameMenu.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/WidgetComponent.h"
#include "Custom/Events/EventDispatcher.h"
#include "UI/Widgets/FindGameWidget.h"
#include "Network/NetworkManager.h"

AFindGameMenu::AFindGameMenu()
{
    PrimaryActorTick.bCanEverTick = false;

    m_widgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("FindGameWidget"));
    ConstructorHelpers::FClassFinder<UUserWidget> widget(TEXT("/Game/FindGameWidget"));
    if (widget.Succeeded())
    {
        m_widgetComponent->SetWidgetClass(widget.Class);
    }
}

void AFindGameMenu::OnReturnToMainMenu(const EventData& eventData)
{
    if (eventData.eventType == GameplayEventType::ReturnToMainMenu)
    {
        m_EventsHandler.unsubscribe();
        Destroy();
    }
}

void AFindGameMenu::OnNoServerConnection(const EventData& eventData)
{
    if (eventData.eventType == GameplayEventType::NoServerConnection)
    {
        if (UFindGameWidget* widget = static_cast<UFindGameWidget*>(m_widgetComponent->GetUserWidgetObject()))
        {
            widget->OnNoConnection(); 
        }
    }
    else if (eventData.eventType == GameplayEventType::ServerConnectionLost)
    {
        if (UFindGameWidget* widget = static_cast<UFindGameWidget*>(m_widgetComponent->GetUserWidgetObject()))
        {
            widget->OnConnectionLost();
        }
    }
}

void AFindGameMenu::OnConnectedToServer(const EventData& eventData)
{
    if (eventData.eventType != GameplayEventType::ConnectedToServer)
    {
        return;
    }

    if (UFindGameWidget* widget = static_cast<UFindGameWidget*>(m_widgetComponent->GetUserWidgetObject()))
    {
        widget->OnConnected();
    }
}

void AFindGameMenu::OnGameFound(const EventData& eventData)
{
    if (eventData.eventType != GameplayEventType::GameFound)
    {
        return;
    }

    if (UFindGameWidget* widget = static_cast<UFindGameWidget*>(m_widgetComponent->GetUserWidgetObject()))
    {
        widget->OnGameFound();
    }
}

void AFindGameMenu::OnAllPlayersReady(const EventData& eventData)
{
    if (eventData.eventType != GameplayEventType::AllPlayersReady)
    {
        return;
    }

    //#TODO implement further game flow!!!
    if (UFindGameWidget* widget = static_cast<UFindGameWidget*>(m_widgetComponent->GetUserWidgetObject()))
    {
        //widget->SetMessageText("All is ready!");
    }
}

void AFindGameMenu::OnWaitingForPlayers(const EventData& eventData)
{
    if (eventData.eventType != GameplayEventType::WaitingForPlayers)
    {
        return;
    }

    if (UFindGameWidget* widget = static_cast<UFindGameWidget*>(m_widgetComponent->GetUserWidgetObject()))
    {
        widget->OnWaitingForPlayers();
    }
}

void AFindGameMenu::OnNotReady(const EventData& eventData)
{
    if (eventData.eventType != GameplayEventType::NotReady)
    {
        return;
    }

    if (UFindGameWidget* widget = static_cast<UFindGameWidget*>(m_widgetComponent->GetUserWidgetObject()))
    {
        widget->OnNotReady();
    }
}

void AFindGameMenu::SubcribeOnEvents()
{
    m_EventsHandler.subscribe({
        {GameplayEventType::ReturnToMainMenu, [this](const EventData& eventData) { OnReturnToMainMenu(eventData); }},
        {GameplayEventType::NoServerConnection, [this](const EventData& eventData) { OnNoServerConnection(eventData); }},
        {GameplayEventType::ConnectedToServer, [this](const EventData& eventData) { OnConnectedToServer(eventData); }},
        {GameplayEventType::GameFound, [this](const EventData& eventData) { OnGameFound(eventData); }},
        {GameplayEventType::AllPlayersReady, [this](const EventData& eventData) { OnAllPlayersReady(eventData); }},
        {GameplayEventType::NotReady, [this](const EventData& eventData) { OnNotReady(eventData); }},
        {GameplayEventType::WaitingForPlayers, [this](const EventData& eventData) { OnWaitingForPlayers(eventData); }},
        {GameplayEventType::ServerConnectionLost, [this](const EventData& eventData) { OnNoServerConnection(eventData); }},
        });
}

void AFindGameMenu::BeginPlay()
{
    Super::BeginPlay();

    SubcribeOnEvents();
    GetWorld()->SpawnActor<ANetworkManager>();
}

void AFindGameMenu::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}

