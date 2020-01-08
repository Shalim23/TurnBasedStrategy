#pragma once
#include <functional>

enum class GameplayEventType
{
    Base,
    ServerConnectionLost,
    LeaveFromMainMenu,
    FindGame,
    ReturnToMainMenu,
    ReadyForGame,
    NoServerConnection,
    ConnectedToServer,
    GameFound,
    AllPlayersReady,
    NotReady,
    WaitingForPlayers,
};

struct EventData
{
    GameplayEventType eventType;

protected:
    EventData(const GameplayEventType& otherEventType)
        : eventType(otherEventType)
    {}
};

struct EventSubscribeData
{
    EventSubscribeData(const GameplayEventType& otherEventType, const std::function<void(const EventData&)>& otherCallback)
        : eventType(otherEventType)
        , callback(otherCallback)
    {}

    GameplayEventType eventType;
    std::function<void(const EventData&)> callback;
};

struct EventDelegateHandle
{
    EventDelegateHandle(const GameplayEventType& otherEventType, const FDelegateHandle& otherHandle)
        : eventType(otherEventType)
        , handle(otherHandle)
    {}

    GameplayEventType eventType;
    FDelegateHandle handle;
};

DECLARE_EVENT_OneParam(EventDispatcher, BaseGameEvent, const struct EventData&)
DECLARE_DERIVED_EVENT(EventDispatcher, BaseGameEvent, ServerConnectionLostEvent)
DECLARE_DERIVED_EVENT(EventDispatcher, BaseGameEvent, LeaveFromMainMenuEvent)
DECLARE_DERIVED_EVENT(EventDispatcher, BaseGameEvent, FindGameEvent)
DECLARE_DERIVED_EVENT(EventDispatcher, BaseGameEvent, ReturnToMainMenuEvent)
DECLARE_DERIVED_EVENT(EventDispatcher, BaseGameEvent, ReadyForGameEvent)
DECLARE_DERIVED_EVENT(EventDispatcher, BaseGameEvent, NoServerConnectionEvent)
DECLARE_DERIVED_EVENT(EventDispatcher, BaseGameEvent, ConnectedToServerEvent)
DECLARE_DERIVED_EVENT(EventDispatcher, BaseGameEvent, GameFoundEvent)
DECLARE_DERIVED_EVENT(EventDispatcher, BaseGameEvent, AllPlayersReadyEvent)
DECLARE_DERIVED_EVENT(EventDispatcher, BaseGameEvent, WaitingForPlayersEvent)
DECLARE_DERIVED_EVENT(EventDispatcher, BaseGameEvent, NotReadyEvent)

//Client events data

struct NotReadyEventData : public EventData
{
    using Super = EventData;

    NotReadyEventData()
        : Super(GameplayEventType::NotReady)
    {}
};

struct WaitingForPlayersEventData : public EventData
{
    using Super = EventData;

    WaitingForPlayersEventData()
        : Super(GameplayEventType::WaitingForPlayers)
    {}
};

struct AllPlayersReadyEventData : public EventData
{
    using Super = EventData;

    AllPlayersReadyEventData()
        : Super(GameplayEventType::AllPlayersReady)
    {}
};

struct GameFoundEventData : public EventData
{
    using Super = EventData;

    GameFoundEventData()
        : Super(GameplayEventType::GameFound)
    {}
};

struct ServerConnectionLostEventData : public EventData
{
    using Super = EventData;

    ServerConnectionLostEventData()
        : Super(GameplayEventType::ServerConnectionLost)
    {}
};

struct LeaveFromMainMenuEventData : public EventData
{
    using Super = EventData;

    LeaveFromMainMenuEventData()
        : Super(GameplayEventType::LeaveFromMainMenu)
    {}
}; 

struct FindGameEventData : public EventData
{
    using Super = EventData;

    FindGameEventData()
        : Super(GameplayEventType::FindGame)
    {}
};

struct ReturnToMainMenuEventData : public EventData
{
    using Super = EventData;

    ReturnToMainMenuEventData()
        : Super(GameplayEventType::ReturnToMainMenu)
    {}
};

struct ReadyForGameEventData : public EventData
{
    using Super = EventData;

    ReadyForGameEventData()
        : Super(GameplayEventType::ReadyForGame)
    {}
}; 

struct NoServerConnectionEventData : public EventData
{
    using Super = EventData;

    NoServerConnectionEventData()
        : Super(GameplayEventType::NoServerConnection)
    {}
}; 

struct ConnectedToServerEventData : public EventData
{
    using Super = EventData;

    ConnectedToServerEventData()
        : Super(GameplayEventType::ConnectedToServer)
    {}
};
