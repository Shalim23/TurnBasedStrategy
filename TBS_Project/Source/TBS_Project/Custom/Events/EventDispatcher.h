#pragma once
#include <memory>
#include <unordered_map>

#include "Custom/Singleton/Singleton.h"
#include "Events.h"

class EventDispatcher : public Singleton<EventDispatcher>
{
    using Super = Singleton;

public:
    void Init() override;

    std::vector<EventDelegateHandle> subscribe(const std::vector<EventSubscribeData>& callbacks);

    void unsubscribe(const std::vector<EventDelegateHandle>& callbacksData);

    BaseGameEvent* GetEvent(const GameplayEventType& eventType);

private:
    std::unordered_map<GameplayEventType, std::unique_ptr<BaseGameEvent>> m_Events;
};
