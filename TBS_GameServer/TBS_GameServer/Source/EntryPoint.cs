
namespace TBS_GameServer.Source.Source
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
            GameServerInstance gameServerInstance = new GameServerInstance();
            gameServerInstance.Init();
            gameServerInstance.Run();
        }
    }
}
