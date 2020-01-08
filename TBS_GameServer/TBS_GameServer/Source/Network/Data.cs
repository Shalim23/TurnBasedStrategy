using System.Net.Sockets;

namespace TBS_GameServer.Network
{
    enum ConnectedSocketState
    {
        WaitingForPlayers,
        Ready,
        Canceled,
        ConnectionLost,
        InvalidData
    }

    class ConnectedPlayerData
    {
        public Socket socket;
        public ConnectedSocketState state;
    }
    
    static class NetworkDataConsts
    {
        public const string Ip = "127.0.0.1"; //#TODO use local for testing only
        public const int Port = 56000;
        public const int MaxPlayers = 1; //#TODO change for 3 and more in future
        public const int DataSize = 1024;
        public const int ReadinessTimeMs = 15000;


        //JSON messages keys
        public const string TBSGameMessageJsonKey = "TBSGameMessage";
        public const string MessageNameJsonKey = "MessageName";

        //messages from server
        public const string WaitingForPlayersMessage = "WaitingForPlayers";
        public const string WaitingForReadinessMessage = "WaitingForReadiness";
        public const string AllAreReadyMessage = "AllAreReady";
        public const string NotReadyMessage = "NotReady";
        public const string TerminatingMessage = "Terminating";

        //messages from client
        public const string IsReadyMessage = "IsReady";
        public const string ClientCancelMessage = "ClientCancel";
    }
}
