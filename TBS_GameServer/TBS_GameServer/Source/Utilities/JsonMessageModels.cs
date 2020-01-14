
namespace TBS_GameServer.Source.Utilities
{
    class JsonNetMessage<T>
    {
        public T tbsGameMessage { get; set; }
    }

    class Message
    {
        public virtual bool IsValid()
        {
            return messageName != null;
        }
        public string messageName { get; set; }
    }

    class ClientConnectionMessage : Message
    {
        public override bool IsValid()
        {
            return base.IsValid() && playersAmount != null;
        }
        
        //public string messageName { get; set; }
        public int? playersAmount { get; set; }
    }
}
