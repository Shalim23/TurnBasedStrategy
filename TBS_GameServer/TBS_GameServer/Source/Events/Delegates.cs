
namespace TBS_GameServer.Source.Events
{
    class Delegates
    {
        public enum DelegateType
        {
            NetworkMessage,
            ConnectionError,
        }

        public delegate void NetworkMessageDelegate(string message, byte[] data);
        public delegate void ConnectionErrorDelegate();
    }
}
