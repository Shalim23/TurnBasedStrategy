#include "EventDispatcher.h"


void EventDispatcher::Init()
{
    m_Events.insert({ GameplayEventType::LeaveFromMainMenu, std::make_unique<LeaveFromMainMenuEvent>() });
    m_Events.insert({ GameplayEventType::ReturnToMainMenu, std::make_unique<ReturnToMainMenuEvent>() });
    m_Events.insert({ GameplayEventType::FindGame, std::make_unique<FindGameEvent>() });
    m_Events.insert({ GameplayEventType::ReadyForGame, std::make_unique<ReadyForGameEvent>() });
    m_Events.insert({ GameplayEventType::NoServerConnection, std::make_unique<NoServerConnectionEvent>() });
    m_Events.insert({ GameplayEventType::ConnectedToServer, std::make_unique<ConnectedToServerEvent>() });
    m_Events.insert({ GameplayEventType::GameFound, std::make_unique<GameFoundEvent>() });
    m_Events.insert({ GameplayEventType::AllPlayersReady, std::make_unique<AllPlayersReadyEvent>() });
    m_Events.insert({ GameplayEventType::NotReady, std::make_unique<NotReadyEvent>() });
    m_Events.insert({ GameplayEventType::ServerConnectionLost, std::make_unique<ServerConnectionLostEvent>() });
    m_Events.insert({ GameplayEventType::WaitingForPlayers, std::make_unique<WaitingForPlayersEvent>() });
    m_Events.insert({ GameplayEventType::CameraZoom, std::make_unique<CameraZoomEvent>() });
}

void EventDispatcher::Shutdown()
{
    m_Events.clear();
}

BaseGameEvent* EventDispatcher::GetEvent(const GameplayEventType& eventType)
{
    auto iter = m_Events.find(eventType);
    if (iter != m_Events.end())
    {
        return iter->second.get();
    }
    
    return nullptr;
}

std::vector<EventDelegateHandle> EventDispatcher::subscribe(const std::vector<EventSubscribeData>& callbacks)
{
    std::vector<EventDelegateHandle> toReturn;
    toReturn.reserve(callbacks.size());

    for (const EventSubscribeData& callbackData : callbacks)
    {
        auto iter = m_Events.find(callbackData.eventType);
        if (iter != m_Events.end())
        {
            toReturn.push_back({ callbackData.eventType,
                iter->second->AddLambda(callbackData.callback) });
        }
    }

    return toReturn;
}

void EventDispatcher::unsubscribe(const std::vector<EventDelegateHandle>& callbacksData)
{
    for (const auto& callbackData : callbacksData)
    {
        auto iter = m_Events.find(callbackData.eventType);
        if (iter != m_Events.end())
        {
            iter->second->Remove(callbackData.handle);
        }
    }
}