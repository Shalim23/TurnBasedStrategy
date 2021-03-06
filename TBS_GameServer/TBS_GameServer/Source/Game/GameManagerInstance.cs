﻿using System.Collections.Generic;
using TBS_GameServer.Source.Events;

using static TBS_GameServer.Source.Game.GameData.GameDataStructs;
using static TBS_GameServer.Source.Events.Delegates;

namespace TBS_GameServer.Source.Game
{
    class GameManagerInstance
    {
        public GameManagerInstance(EventsManagerInstance eventsManager, int playersAmount)
        {
            m_EventsManager = eventsManager;
            SubscribeOnEvents();

            InitGameData(playersAmount);
        }
        void SubscribeOnEvents()
        {
            //m_EventsManager.Subscribe<PlayersConnectedDelegate>(DelegateType.PlayersConnected, OnPlayersConnected);
        }

        void InitGameData(int playersAmount)
        {
            //#TODO init game data
        }

        EventsManagerInstance m_EventsManager;
        Dictionary<string, PlayerData> m_Players;
    }
}
