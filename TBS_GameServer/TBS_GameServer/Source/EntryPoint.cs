
namespace TBS_GameServer.Source.Source
{
    class EntryPoint
    {
        static void Main(string[] args)
        {
            GameServerInstance gameServerInstance = new GameServerInstance();
            gameServerInstance.Run();
        }
    }
}
