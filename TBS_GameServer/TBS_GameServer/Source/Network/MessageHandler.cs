using System;
using System.Collections.Generic;
using System.Text.Json;
using TBS_GameServer.Events;

namespace TBS_GameServer.Network
{
    class MessageHandler
    {
        public void Init()
        {
            SubscribeOnEvents();
            //#TODO init callbacks for gameplay events
        }
        void ProcessMessage(JsonElement message)
        {
            //#TODO deserialize and process;
        }

        void SubscribeOnEvents()
        {
            EventsManager eventsManager = EventsManager.GetInstance();
            eventsManager.Subscribe<Delegates.NetworkMessageDelegate>(Delegates.DelegateType.NetworkMessage, ProcessMessage);
        }

        
        Dictionary<string, Func<string>> m_NetEventsCallbacks;
    }
}
