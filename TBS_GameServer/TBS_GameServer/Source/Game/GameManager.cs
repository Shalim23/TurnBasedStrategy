using System.Collections.Generic;
using System.Runtime.CompilerServices;
using TBS_GameServer.Game.GameData;
using TBS_GameServer.Events;
using static TBS_GameServer.Events.Delegates;

namespace TBS_GameServer.Game
{
    class GameManager
    {
        static GameManager instance = null;
        [MethodImpl(MethodImplOptions.Synchronized)]
        public static GameManager GetInstance()
        {
            if (instance == null)
            {
                instance = new GameManager();
            }

            return instance;
        }

        void SubscribeOnEvents()
        {
            EventsManager eventsManager = EventsManager.GetInstance();

            eventsManager.Subscribe<PlayersConnectedDelegate>(DelegateType.PlayersConnected, OnPlayersConnected);
        }

        public void Init()
        {
            SubscribeOnEvents();
        }

        public void CleanUp()
        {
            m_Players = null;
        }

        void OnPlayersConnected(int playersAmount)
        {
            
        }

        Dictionary<string, GameDataStructs.PlayerData> m_Players;
    }
}
