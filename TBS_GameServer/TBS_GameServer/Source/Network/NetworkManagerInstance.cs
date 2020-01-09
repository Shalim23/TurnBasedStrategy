using TBS_GameServer.Network.PlayerConnection;
using System.Runtime.CompilerServices;
using System.Text.Json;
using System.Collections.Generic;
using TBS_GameServer.Events;

using static TBS_GameServer.Events.Delegates;


namespace TBS_GameServer.Network
{
    class NetworkManagerInstance
    {
        void SubscribeOnEvents()
        {
            m_EventsManager.Subscribe<ConnectionErrorDelegate>(DelegateType.ConnectionError, OnConnectionError);
        }

        public void Init(EventsManagerInstance eventsManager, List<ConnectedPlayerData> connectedPlayers)
        {
            m_EventsManager = eventsManager;
            SubscribeOnEvents();

            m_MessageHandler = new MessageHandler();
            m_MessageHandler.Init();

            m_SocketsHandler = new SocketsHandler();
            m_SocketsHandler.Activate(connectedPlayers);

            m_IsActive = true;
        }

        public void StartMessageProcessing()
        {
            while(m_IsActive)
            {
                m_SocketsHandler.ProcessReceive();
            }
        }

        void OnConnectionError()
        {
            m_SocketsHandler.SendConnectionError();
            m_IsActive = false;
        }

        EventsManagerInstance m_EventsManager = null;
        SocketsHandler m_SocketsHandler = null;
        MessageHandler m_MessageHandler = null;

        bool m_IsActive = false;
    }
}
