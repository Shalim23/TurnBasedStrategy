using System.Collections.Generic;
using TBS_GameServer.Utilities;
using TBS_GameServer.Events;

using static TBS_GameServer.Game.GameData.GameDataStructs;
using static TBS_GameServer.Events.Delegates;

namespace TBS_GameServer.Game
{
    class GameManagerInstance
    {
        void SubscribeOnEvents()
        {
            m_EventsManager.Subscribe<PlayersConnectedDelegate>(DelegateType.PlayersConnected, OnPlayersConnected);
        }

        public void Init(EventsManagerInstance eventsManager)
        {
            m_EventsManager = eventsManager;
            SubscribeOnEvents();
        }

        void OnPlayersConnected()
        {
            
        }

        EventsManagerInstance m_EventsManager;
        Dictionary<string, PlayerData> m_Players;
    }
}
