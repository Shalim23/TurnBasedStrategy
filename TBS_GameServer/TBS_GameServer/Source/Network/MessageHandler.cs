using System;
using System.Collections.Generic;
using TBS_GameServer.Source.Events;

using static TBS_GameServer.Source.Events.Delegates;

namespace TBS_GameServer.Source.Network
{
    class MessageHandler
    {
        public MessageHandler(EventsManagerInstance eventsManager)
        {
            m_EventsManager = eventsManager;
            SubscribeOnEvents();

            m_NetEventsCallbacks = new Dictionary<string, Func<string>>();

            //#TODO init callbacks for gameplay events
        }

        void ProcessMessage(string message, byte[] data)
        {
            if(m_NetEventsCallbacks.ContainsKey(message))
            {
                //#TODO deserialize and process;
            }
            else
            {
                Console.WriteLine($"{message} -> not present in MessageHandler");
            }
        }

        void SubscribeOnEvents()
        {
            m_EventsManager.Subscribe<NetworkMessageDelegate>(DelegateType.NetworkMessage, ProcessMessage);
        }

        EventsManagerInstance m_EventsManager = null;
        Dictionary<string, Func<string>> m_NetEventsCallbacks = null;
    }
}
