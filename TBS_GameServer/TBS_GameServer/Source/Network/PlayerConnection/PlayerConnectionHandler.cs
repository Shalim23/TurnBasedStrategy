using System.Collections.Generic;

namespace TBS_GameServer.Network.PlayerConnection
{
    class PlayerConnectionHandler
    {
        public List<ConnectedPlayerData> GetReadyPlayers()
        {
            List<ConnectedPlayerData> readyPlayers = new List<ConnectedPlayerData>();
            bool arePlayersConnected = false;

            while (!arePlayersConnected)
            {
                List<ConnectedPlayerData> connectedPlayers = m_Listener.WaitForPlayersConnection(readyPlayers);

                readyPlayers = m_ReadinessHandler.WaitForPlayersReadiness(connectedPlayers);

                if(readyPlayers.Count == NetworkDataConsts.MaxPlayers)
                {
                    arePlayersConnected = true;
                }
            }

            return readyPlayers;
        }
        
        PlayersListener m_Listener = new PlayersListener();
        PlayersReadinessHandler m_ReadinessHandler = new PlayersReadinessHandler();
    }
}
