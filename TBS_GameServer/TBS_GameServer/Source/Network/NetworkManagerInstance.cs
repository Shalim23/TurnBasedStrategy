﻿using System.Collections.Generic;
using TBS_GameServer.Source.Events;

using static TBS_GameServer.Source.Events.Delegates;


namespace TBS_GameServer.Source.Network
{
    class NetworkManagerInstance
    {
        public NetworkManagerInstance(EventsManagerInstance eventsManager,
            List<ConnectedPlayerData> connectedPlayers)
        {
            m_EventsManager = eventsManager;
            SubscribeOnEvents();

            m_MessageHandler = new MessageHandler(m_EventsManager);
            m_SocketsHandler = new SocketsHandler(m_EventsManager, connectedPlayers);
        }
        void SubscribeOnEvents()
        {
            m_EventsManager.Subscribe<ConnectionErrorDelegate>(DelegateType.ConnectionError, OnConnectionError);
        }

        public void StartMessageProcessing()
        {
            m_SocketsHandler.ProcessReceive();
        }

        void OnConnectionError()
        {
            m_SocketsHandler.SendConnectionError();
        }

        EventsManagerInstance m_EventsManager = null;
        SocketsHandler m_SocketsHandler = null;
        MessageHandler m_MessageHandler = null;
    }
}
