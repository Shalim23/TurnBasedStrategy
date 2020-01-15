#pragma once
#include <functional>

class FBufferArchive;
class FMemoryReader;

enum class GameplayEventType
{
    Base,
    LeaveFromMainMenu,
    ReturnToMainMenu,
    FindGame,
    ReadyForGame,
    NoServerConnection,
    ConnectedToServer,
    GameFound,
    AllPlayersReady,
    NotReady,
    ServerConnectionLost,
    WaitingForPlayers,
    CameraZoom
};

struct EventData
{
    GameplayEventType eventType;

public:
    virtual void Serialize(FBufferArchive& data) {}
    virtual void Deserialize(FMemoryReader& data) {}

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
DECLARE_DERIVED_EVENT(EventDispatcher, BaseGameEvent, LeaveFromMainMenuEvent)
DECLARE_DERIVED_EVENT(EventDispatcher, BaseGameEvent, ReturnToMainMenuEvent)
DECLARE_DERIVED_EVENT(EventDispatcher, BaseGameEvent, FindGameEvent)
DECLARE_DERIVED_EVENT(EventDispatcher, BaseGameEvent, ReadyForGameEvent)
DECLARE_DERIVED_EVENT(EventDispatcher, BaseGameEvent, NoServerConnectionEvent)
DECLARE_DERIVED_EVENT(EventDispatcher, BaseGameEvent, ConnectedToServerEvent)
DECLARE_DERIVED_EVENT(EventDispatcher, BaseGameEvent, GameFoundEvent)
DECLARE_DERIVED_EVENT(EventDispatcher, BaseGameEvent, AllPlayersReadyEvent)
DECLARE_DERIVED_EVENT(EventDispatcher, BaseGameEvent, NotReadyEvent)
DECLARE_DERIVED_EVENT(EventDispatcher, BaseGameEvent, ServerConnectionLostEvent)
DECLARE_DERIVED_EVENT(EventDispatcher, BaseGameEvent, WaitingForPlayersEvent)
DECLARE_DERIVED_EVENT(EventDispatcher, BaseGameEvent, CameraZoomEvent)

//Client events data

struct CameraZoomEventData : public EventData
{
    using Super = EventData;

    CameraZoomEventData(float _scaleValue)
        : Super(GameplayEventType::CameraZoom)
        , scaleValue(_scaleValue)
    {}

    float scaleValue = 0.0f;
};

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

struct ServerConnectionLostEventData : public EventData
{
    using Super = EventData;

    ServerConnectionLostEventData()
        : Super(GameplayEventType::ServerConnectionLost)
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

struct LeaveFromMainMenuEventData : public EventData
{
    using Super = EventData;

    LeaveFromMainMenuEventData()
        : Super(GameplayEventType::LeaveFromMainMenu)
    {}
};

struct ReturnToMainMenuEventData : public EventData
{
    using Super = EventData;

    ReturnToMainMenuEventData()
        : Super(GameplayEventType::ReturnToMainMenu)
    {}
};

struct FindGameEventData : public EventData
{
    using Super = EventData;

    FindGameEventData(int _playerAmountToSearch)
        : Super(GameplayEventType::FindGame)
        , playerAmountToSearch(_playerAmountToSearch)
    {}

    int playerAmountToSearch = 0;
};

struct ReadyForGameEventData : public EventData
{
    using Super = EventData;

    ReadyForGameEventData()
        : Super(GameplayEventType::ReadyForGame)
    {}
};
