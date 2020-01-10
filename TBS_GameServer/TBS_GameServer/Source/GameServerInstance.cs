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
            m_PlayersListener.Init();
            

            m_PlayersReadinessHandler = new PlayersReadinessHandler();
            m_RoomsHandler = new GameRoomsHandler();


        }

        public void Run()
        {
            while (true)
            {
                m_PlayersListener.Run();
            }
        }

        PlayersListener m_PlayersListener = null;
        PlayersReadinessHandler m_PlayersReadinessHandler = null;
        GameRoomsHandler m_RoomsHandler = null;
    }
}
