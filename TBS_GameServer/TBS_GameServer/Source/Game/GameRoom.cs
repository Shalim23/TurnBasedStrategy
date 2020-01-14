using System.Collections.Generic;

using TBS_GameServer.Source.Network;
using TBS_GameServer.Source.Events;

namespace TBS_GameServer.Source.Game
{
    class GameRoom
    {
        public delegate void OnRoomShutdown(string RoomId);

        public GameRoom(string roomId, List<ConnectedPlayerData> connectedPlayers,
            OnRoomShutdown onRoomShutdownCallback)
        {
            m_RoomId = roomId;
            m_OnRoomShutdownCallback = onRoomShutdownCallback;

            m_EventsManager = new EventsManagerInstance();
            m_EventsManager.Init();

            m_GameInstance = new GameManagerInstance(m_EventsManager, connectedPlayers.Count);
            m_NetworkManager = new NetworkManagerInstance(m_EventsManager, connectedPlayers);
        }

        public void Run()
        {
            m_NetworkManager.StartMessageProcessing();

            Shutdown();
        }

        void Shutdown()
        {
            m_OnRoomShutdownCallback(m_RoomId);
        }

        OnRoomShutdown m_OnRoomShutdownCallback = null;

        EventsManagerInstance m_EventsManager = null;
        GameManagerInstance m_GameInstance = null;
        NetworkManagerInstance m_NetworkManager = null;

        string m_RoomId = null;
    }
}
