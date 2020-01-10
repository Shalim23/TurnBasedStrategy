using System.Collections.Generic;
using System.Threading.Tasks;

using TBS_GameServer.Source.Game;
using TBS_GameServer.Source.Network;
using TBS_GameServer.Source.Utilities;

namespace TBS_GameServer.Source
{
    class GameServerInstance
    {
        public void Init()
        {
            JsonDataLoader.LoadJsonData();

            m_PlayersListener = new PlayersListener();
            m_PlayersListener.Init(connectedPlayers => OnPlayersConnected(connectedPlayers));
            
            m_RoomsHandler = new GameRoomsHandler();
        }

        public void Run()
        {
            while (true)
            {
                m_PlayersListener.Run();
            }
        }

        void OnPlayersConnected(List<ConnectedPlayerData> connectedPlayers)
        {
            PlayersReadinessHandler readinessHandler = new PlayersReadinessHandler();
            readinessHandler.Init(connectedPlayers,
                players => OnPlayersReady(players), players => OnRestartConnection(players));

            Task.Run(() => readinessHandler.Run());
        }

        void OnPlayersReady(List<ConnectedPlayerData> connectedPlayers)
        {
            m_RoomsHandler.CreateRoom(connectedPlayers);
        }

        void OnRestartConnection(List<ConnectedPlayerData> connectedPlayers)
        {
            m_PlayersListener.OnRestartAfterReadinessFailed(connectedPlayers);
        }

        ///////////////////////////////////////////////////////////////

        PlayersListener m_PlayersListener = null;
        GameRoomsHandler m_RoomsHandler = null;
    }
}
