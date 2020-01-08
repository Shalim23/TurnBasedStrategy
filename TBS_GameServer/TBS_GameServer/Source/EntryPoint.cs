using TBS_GameServer.Network;
using TBS_GameServer.Game;
using TBS_GameServer.Events;

namespace TBS_GameServer
{
    class EntryPoint
    {
        static void Main(string[] args)
        {
            Utilities.JsonDataLoader.LoadCardsData();

            Init();

            NetworkManager networkManager = NetworkManager.GetInstance();

            networkManager.ProcessPlayerConnection();
            networkManager.StartMessageProcessing();

            CleanUp();
        }

        static void Init()
        {
            EventsManager.GetInstance().Init();
            NetworkManager.GetInstance().Init();
            GameManager.GetInstance().Init();
        }

        static void CleanUp()
        {
            NetworkManager.GetInstance().CleanUp();
            GameManager.GetInstance().CleanUp();
            EventsManager.GetInstance().CleanUp();
        }
    }
}
