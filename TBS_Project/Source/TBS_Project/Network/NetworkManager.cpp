#include "NetworkManager.h"
#include "Custom/Events/Events.h"
#include "Custom/Events/EventDispatcher.h"
#include "Serialization/JsonSerializer.h"

ANetworkManager::ANetworkManager()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ANetworkManager::BeginPlay()
{
    Super::BeginPlay();
    SetActorTickEnabled(false);

    SubscribeOnEvents();

    m_NetworkSocketHandler.SetMessageFromServerCallback(
        [this](const TSharedPtr<FJsonObject>& jsonObject) { ProcessMessage(jsonObject); });

    m_NetworkSocketHandler.SetSendErrorCallback(
        [this]() { OnConnectionError(); });

    m_NetworkMessagesHandler.Init();

    Connect();
}

void ANetworkManager::OnReturnToMainMenu(const EventData& eventData)
{
    if (eventData.eventType == GameplayEventType::ReturnToMainMenu)
    {
        if (m_IsConnected)
        {
            TSharedPtr<FJsonObject> messageObject = MakeShareable(new FJsonObject);
            messageObject->SetStringField(MessageNameJsonKey, ClientCancelMessage);

            m_NetworkSocketHandler.Send(messageObject);
        }

        Shutdown();
    }
}

void ANetworkManager::OnPlayerReady(const EventData& eventData)
{
    if (eventData.eventType == GameplayEventType::ReadyForGame)
    {
        TSharedPtr<FJsonObject> messageObject = MakeShareable(new FJsonObject);
        messageObject->SetStringField(MessageNameJsonKey, IsReadyMessage);

        m_NetworkSocketHandler.Send(messageObject);
    }
}

void ANetworkManager::Connect()
{
    if (m_NetworkSocketHandler.TryConnect())
    {
        m_IsConnected = true;
        SetActorTickEnabled(true);

        if (BaseGameEvent* connectedToServerEvent =
            EventDispatcher::GetInstance().GetEvent(GameplayEventType::ConnectedToServer))
        {
            connectedToServerEvent->Broadcast(ConnectedToServerEventData());
        }
    }
    else
    {
        if (BaseGameEvent* noServerConnectionEvent =
            EventDispatcher::GetInstance().GetEvent(GameplayEventType::NoServerConnection))
        {
            noServerConnectionEvent->Broadcast(NoServerConnectionEventData());
        }
    }
}

void ANetworkManager::Shutdown()
{
    m_NetworkSocketHandler.Shutdown();
    m_EventsHandler.unsubscribe();
    Destroy();
}

void ANetworkManager::SubscribeOnEvents()
{
    m_EventsHandler.subscribe({
        {GameplayEventType::ReturnToMainMenu, [this](const EventData& eventData) { OnReturnToMainMenu(eventData); }},
        {GameplayEventType::ReadyForGame, [this](const EventData& eventData) { OnPlayerReady(eventData); }},
        });
}

void ANetworkManager::ProcessMessage(const TSharedPtr<FJsonObject>& jsonObject)
{
    m_NetworkMessagesHandler.ProcessMessage(jsonObject);
}

void ANetworkManager::OnConnectionError()
{
    if (BaseGameEvent* serverConnectionLostEvent =
        EventDispatcher::GetInstance().GetEvent(GameplayEventType::ServerConnectionLost))
    {
        serverConnectionLostEvent->Broadcast(ServerConnectionLostEventData());
    }

    Shutdown();
}

void ANetworkManager::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    m_NetworkSocketHandler.Receive();
}
