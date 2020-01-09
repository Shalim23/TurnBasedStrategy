using System;
using System.Collections.Generic;
using System.Text;
using TBS_GameServer.Network;
using TBS_GameServer.Game;
using TBS_GameServer.Events;

namespace TBS_GameServer.Source.Game
{
    class GameRoom
    {
        public void Init(List<ConnectedPlayerData> connectedPlayers)
        {
            m_EventsManager = new EventsManagerInstance();
            m_EventsManager.Init();

            m_GameInstance = new GameManagerInstance();
            m_GameInstance.Init(m_EventsManager);
        }

        EventsManagerInstance m_EventsManager = null;
        GameManagerInstance m_GameInstance = null;
    }
}
