using TBS_GameServer.Network;
using TBS_GameServer.Game;
using TBS_GameServer.Events;
using TBS_GameServer.Utilities;

namespace TBS_GameServer
{
    class EntryPoint
    {
        /*#TODO game room class with game instance, net instance, events instance
         * pass events instance to game and net
         * run as Task
         * global player connector while loop
         */
        static void Main(string[] args)
        {
            JsonDataLoader.LoadJsonData();

            Init();

            NetworkManagerInstance networkManager = NetworkManagerInstance.GetInstance();

            networkManager.ProcessPlayerConnection();
            networkManager.StartMessageProcessing();

            CleanUp();
        }

        static void Init()
        {
            EventsManagerInstance.GetInstance().Init();
            NetworkManagerInstance.GetInstance().Init();
            GameManagerInstance.GetInstance().Init();
        }

        static void CleanUp()
        {
            NetworkManagerInstance.GetInstance().CleanUp();
            GameManagerInstance.GetInstance().CleanUp();
            EventsManagerInstance.GetInstance().CleanUp();
        }
    }
}
