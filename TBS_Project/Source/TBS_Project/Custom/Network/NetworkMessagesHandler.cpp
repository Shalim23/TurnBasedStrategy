#include "NetworkMessagesHandler.h"
#include "NetworkData.h"
#include "Custom/Events/EventDispatcher.h"
#include "Serialization/JsonSerializer.h"

void NetworkMessagesHandler::Init()
{
    m_NetEventsCallbacks.Add(WaitingForPlayersMessageName,
        [this](const TSharedPtr<FJsonObject>& jsonObject) { OnNetWaitingForPlayersMessage(jsonObject); });

    m_NetEventsCallbacks.Add(AllAreReadyMessageName,
        [this](const TSharedPtr<FJsonObject>& jsonObject) { OnNetAllAreReadyMessage(jsonObject); });

    m_NetEventsCallbacks.Add(WaitingForReadinessMessageName,
        [this](const TSharedPtr<FJsonObject>& jsonObject) { OnNetWaitingForReadinessMessage(jsonObject); });

    m_NetEventsCallbacks.Add(NotReadyMessageName,
        [this](const TSharedPtr<FJsonObject>& jsonObject) { OnNetNotReadyMessage(jsonObject); });
}

void NetworkMessagesHandler::ProcessMessage(const TSharedPtr<FJsonObject>& jsonObject)
{
    FString messageName;
    TSharedPtr<FJsonValue> messageNameField = jsonObject->TryGetField(MessageNameJsonKey);
    if (messageNameField && messageNameField->TryGetString(messageName))
    {
        if (auto callback = m_NetEventsCallbacks.Find(messageName))
        {
            (*callback)(jsonObject);
        }
    }
}

void NetworkMessagesHandler::OnNetAllAreReadyMessage(const TSharedPtr<FJsonObject>& jsonObject)
{
    if (BaseGameEvent* allPlayersReadyEvent =
        EventDispatcher::GetInstance().GetEvent(GameplayEventType::AllPlayersReady))
    {
        allPlayersReadyEvent->Broadcast(AllPlayersReadyEventData());
    }
}

void NetworkMessagesHandler::OnNetWaitingForReadinessMessage(const TSharedPtr<FJsonObject>& jsonObject)
{
    if (BaseGameEvent* gameFoundEvent =
        EventDispatcher::GetInstance().GetEvent(GameplayEventType::GameFound))
    {
        gameFoundEvent->Broadcast(GameFoundEventData());
    }
}

void NetworkMessagesHandler::OnNetWaitingForPlayersMessage(const TSharedPtr<FJsonObject>& jsonObject)
{
    if (BaseGameEvent* waitingForPlayersEvent =
        EventDispatcher::GetInstance().GetEvent(GameplayEventType::WaitingForPlayers))
    {
        waitingForPlayersEvent->Broadcast(WaitingForPlayersEventData());
    }
}

void NetworkMessagesHandler::OnNetNotReadyMessage(const TSharedPtr<FJsonObject>& jsonObject)
{
    if (BaseGameEvent* notReadyEvent =
        EventDispatcher::GetInstance().GetEvent(GameplayEventType::NotReady))
    {
        notReadyEvent->Broadcast(NotReadyEventData());
    }
}