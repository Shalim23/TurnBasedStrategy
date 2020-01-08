#include "EventDispatcher.h"


void EventDispatcher::Init()
{
    //m_Events.insert({ GameplayEventType::LeaveFromMainMenu, std::make_unique<LeaveFromMainMenuEvent>() });
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