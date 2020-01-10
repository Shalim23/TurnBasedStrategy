using System.Text.Json;

namespace TBS_GameServer.Source.Events
{
    class Delegates
    {
        public enum DelegateType
        {
            NetworkMessage,
            ConnectionError,
        }

        public delegate void NetworkMessageDelegate(JsonElement data);
        public delegate void ConnectionErrorDelegate();
    }
}
