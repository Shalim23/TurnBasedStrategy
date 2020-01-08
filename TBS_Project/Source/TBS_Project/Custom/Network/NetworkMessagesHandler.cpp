#include "NetworkMessagesHandler.h"
#include "NetworkData.h"
#include "Custom/Events/EventDispatcher.h"
#include "Serialization/JsonSerializer.h"

void NetworkMessagesHandler::Init()
{
    /*m_NetEventsCallbacks.Add(WaitingForPlayersMessage,
        [this](const TSharedPtr<FJsonObject>& jsonObject) { OnNetWaitingForPlayersMessage(jsonObject); });*/
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

//void NetworkMessagesHandler::OnNetAllAreReadyMessage(const TSharedPtr<FJsonObject>& jsonObject)
//{
//    if (BaseGameEvent* allPlayersReadyEvent =
//        EventDispatcher::GetInstance().GetEvent(GameplayEventType::AllPlayersReady))
//    {
//        allPlayersReadyEvent->Broadcast(AllPlayersReadyEventData());
//    }
//}
