using System.Net.Sockets;
using System.Collections.Generic;

namespace TBS_GameServer.Source.Network
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
        public int searchedPlayersAmount;
    }

    class ConnectedPlayers
    {
        public int Amount;
        public List<ConnectedPlayerData> connectedPlayers;
    }
    
    static class NetworkDataConsts
    {
        public const string Ip = "127.0.0.1"; //#TODO use local for testing only
        public const int Port = 56000;
        public const int MaxPlayers = 8;
        public const int MinPlayers = 2;
        public const int DataSize = 1024;
        public const int ReadinessTimeMs = 15000;


        //JSON messages keys
        public const string TBSGameMessageJsonKey = "TBSGameMessage";
        public const string MessageNameJsonKey = "MessageName";
        public const string PlayersAmountJsonKey = "PlayersAmount";

        //messages from server
        public const string WaitingForPlayersMessage = "WaitingForPlayers";
        public const string WaitingForReadinessMessage = "WaitingForReadiness";
        public const string AllAreReadyMessage = "AllAreReady";
        public const string NotReadyMessage = "NotReady";
        public const string TerminatingMessage = "Terminating";

        //messages from client
        public const string IsReadyMessage = "IsReady";
        public const string ClientCancelMessage = "ClientCancel";
        public const string ClientConnectionMessage = "ClientConnection";
    }
}
