
namespace TBS_GameServer.Source.Utilities
{
    class JsonNetMessage<T>
    {
        public T tbsGameMessage { get; set; }
    }

    class Message
    {
        public string messageName { get; set; }
    }

    class ClientConnectionMessage
    {
        public string messageName { get; set; }
        public int? playersAmount { get; set; }
    }
}
