
namespace TBS_GameServer.Source.Network
{
    static class NetworkHelper
    {
        static public void QueueUserToRemove(ConnectedPlayerData connectedPlayerData, ConnectedSocketState newState)
        {
            connectedPlayerData.state = newState;
            connectedPlayerData.socket.Close();
        }
    }
}
