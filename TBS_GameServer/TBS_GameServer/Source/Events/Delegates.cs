using System.Text.Json;

namespace TBS_GameServer.Events
{
    class Delegates
    {
        public enum DelegateType
        {
            NetworkMessage,
            ConnectionError,
            PlayersConnected
        }

        public delegate void NetworkMessageDelegate(JsonElement data);
        public delegate void ConnectionErrorDelegate();
        public delegate void PlayersConnectedDelegate();
    }
}
