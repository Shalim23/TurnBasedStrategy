using System;
using System.Collections.Generic;
using TBS_GameServer.Source.Events;

using static TBS_GameServer.Source.Events.Delegates;

namespace TBS_GameServer.Source.Network
{
    class MessageHandler
    {
        public void Init(EventsManagerInstance eventsManager)
        {
            m_EventsManager = eventsManager;
            SubscribeOnEvents();

            //#TODO init callbacks for gameplay events
        }
        void ProcessMessage(string message, byte[] data)
        {
            //#TODO deserialize and process;
        }

        void SubscribeOnEvents()
        {
            m_EventsManager.Subscribe<NetworkMessageDelegate>(DelegateType.NetworkMessage, ProcessMessage);
        }

        EventsManagerInstance m_EventsManager = null;
        Dictionary<string, Func<string>> m_NetEventsCallbacks;
    }
}
