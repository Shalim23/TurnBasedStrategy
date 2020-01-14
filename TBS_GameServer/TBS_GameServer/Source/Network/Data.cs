using System.Net.Sockets;

namespace TBS_GameServer.Source.Network
{
    public enum ConnectedSocketState
    {
        WaitingForPlayers,
        Ready,
        Canceled,
        ConnectionLost,
        InvalidData
    }

    public class ConnectedPlayerData
    {
        public ConnectedPlayerData(Socket _socket, ConnectedSocketState _state,
            int _searchedPlayersAmount)
        {
            socket = _socket;
            state = _state;
            searchedPlayersAmount = _searchedPlayersAmount;
        }
    
        public Socket socket;
        public ConnectedSocketState state;
        public int searchedPlayersAmount;
    }
    
    static class NetworkDataConsts
    {
        public const string Ip = "127.0.0.1"; //#TODO use local for testing only
        public const int Port = 56000;
        public const int DataSize = 1024;

        public const int MaxPlayers = 8;
        public const int MinPlayers = 2;
        public const int NewConnectedUserMessageTimeLimit = 5;

        public const int ReadyTimeLimitSec = 15;

        //messages from server
        public const string WaitingForPlayersMessageName = "WaitingForPlayers";
        public const string WaitingForReadinessMessageName = "WaitingForReadiness";
        public const string AllAreReadyMessageName = "AllAreReady";
        public const string NotReadyMessageName = "NotReady";
        public const string TerminatingMessageName = "Terminating";

        //messages from client
        public const string IsReadyMessageName = "IsReady";
        public const string ClientCancelMessageName = "ClientCancel";
        public const string ClientConnectionMessageName = "ClientConnection";
    }
}
