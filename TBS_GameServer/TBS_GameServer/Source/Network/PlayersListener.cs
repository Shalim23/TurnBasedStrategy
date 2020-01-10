using System.Net.Sockets;
using System.Net;
using System;
using System.Collections.Generic;
using System.Threading.Tasks;

using TBS_GameServer.Source.Utilities;
using static TBS_GameServer.Source.Network.NetworkHelper;


namespace TBS_GameServer.Source.Network
{
    public delegate void OnPlayersConnected(List<ConnectedPlayerData> connectedPlayers);

    class PlayersListener
    {
        public void Init(OnPlayersConnected onPlayersConnectedCallback)
        {
            m_Listener =
                new TcpListener(IPAddress.Parse(NetworkDataConsts.Ip), NetworkDataConsts.Port);

            m_PendingPlayers = new Dictionary<int, List<ConnectedPlayerData>>();
            m_PendingPlayersAfterConnectionError = new List<ConnectedPlayerData>();

            m_OnPlayersConnectedCallback = onPlayersConnectedCallback;

            m_IsActive = true;
        }

        public void Run()
        {
            m_Listener.Start();

            Console.WriteLine($"Started listening...");
            while (m_IsActive)
            {
                Socket handler = TryAcceptUserConnection();
                if (handler != null)
                {
                    ProcessNewConnectedUser(handler);
                }

                CheckCancelFromPlayers();
                CheckPendingUsersAfterConnectionError();
            }
        }

        public void OnRestartAfterReadinessFailed(List<ConnectedPlayerData> connectedPlayers)
        {
            lock(ListenerLock)
            {
                m_PendingPlayersAfterConnectionError.AddRange(connectedPlayers);
            }

            m_HasPendingUsersAfterConnectionError = true;
        }

        void AddNewConnectedUser(int searchedPlayerAmount, Socket handler)
        {
            List<ConnectedPlayerData> pendingPlayersForSearchedAmount;
            if(m_PendingPlayers.TryGetValue(searchedPlayerAmount, out pendingPlayersForSearchedAmount))
            {
                ConnectedPlayerData connectedPlayerData = new ConnectedPlayerData();
                connectedPlayerData.state = ConnectedSocketState.WaitingForPlayers;
                connectedPlayerData.socket = handler;
                connectedPlayerData.searchedPlayersAmount = searchedPlayerAmount;

                pendingPlayersForSearchedAmount.Add(connectedPlayerData);
                Console.WriteLine($"{pendingPlayersForSearchedAmount.Count.ToString()} player(s) for {searchedPlayerAmount} players room");

                if (searchedPlayerAmount == pendingPlayersForSearchedAmount.Count)
                {
                    Console.WriteLine($"Readiness check for {searchedPlayerAmount} players room");
                    FinalizeConnection(pendingPlayersForSearchedAmount);

                    Task.Run(() => m_OnPlayersConnectedCallback(pendingPlayersForSearchedAmount));
                    m_PendingPlayers.Remove(searchedPlayerAmount);
                }
            }
            else
            {
                List<ConnectedPlayerData> newPendingPlayerForSearchedAmount =
                    new List<ConnectedPlayerData>();

                ConnectedPlayerData connectedPlayerData = new ConnectedPlayerData();
                connectedPlayerData.state = ConnectedSocketState.WaitingForPlayers;
                connectedPlayerData.socket = handler;
                connectedPlayerData.searchedPlayersAmount = searchedPlayerAmount;
             
                newPendingPlayerForSearchedAmount.Add(connectedPlayerData);
                Console.WriteLine($"{pendingPlayersForSearchedAmount.Count.ToString()} player(s) for {searchedPlayerAmount} players room");

                m_PendingPlayers.Add(searchedPlayerAmount, newPendingPlayerForSearchedAmount);
            }
        }
 
        void ProcessNewConnectedUser(Socket handler)
        {
            byte[] buffer = new byte[NetworkDataConsts.DataSize];
            SocketError socketError;

            if (handler.Receive(buffer, 0, NetworkDataConsts.DataSize, SocketFlags.None, out socketError) > 0)
            {
                int searchedPlayerAmount = 0;
                if(TryParseNewConnectedUserMessage(buffer, out searchedPlayerAmount))
                {
                    AddNewConnectedUser(searchedPlayerAmount, handler);
                }
            }
            else
            {
                Console.WriteLine("ProcessNewConnectedUser -> no data was sent on connection");
            }
        }
        
        private void FinalizeConnection(List<ConnectedPlayerData> connectedUsers)
        {
            Dictionary<string, object> values = new Dictionary<string, object>();
            values.Add(NetworkDataConsts.MessageNameJsonKey, NetworkDataConsts.WaitingForReadinessMessage);
            byte[] buffer = Utils.JsonMessagePacker(values);
            
            foreach (ConnectedPlayerData user in connectedUsers)
            {
                SocketError socketError;

                user.socket.Send(buffer, 0, buffer.Length, SocketFlags.None, out socketError);
                if (socketError == SocketError.ConnectionReset)
                {
                    Console.WriteLine("WaitForUserConnection -> connected user fail");
                    Utils.QueueUserToRemove(user, ConnectedSocketState.ConnectionLost);
                }
            }

            RemoveInactiveUsers();
        }

        private Socket TryAcceptUserConnection()
        {
            Socket handler = null;
            if (m_Listener.Pending())
            {
                handler = m_Listener.AcceptSocket();
                handler.Blocking = false;
            }

            return handler;
        }

        void CheckCancelFromPlayers()
        {
            foreach (KeyValuePair<int, List<ConnectedPlayerData>> players in m_PendingPlayers)
            {
                foreach(ConnectedPlayerData player in players.Value)
                {
                    byte[] buffer = new byte[NetworkDataConsts.DataSize];
                    SocketError socketError;

                    int receivesBytes = player.socket.Receive(buffer, 0, NetworkDataConsts.DataSize, SocketFlags.None, out socketError);
                    if (receivesBytes > 0)
                    {
                        if (TryParseCancelMessage(buffer))
                        {
                            Console.WriteLine("CheckCancelFromPlayers -> cancel from player");
                            Utils.QueueUserToRemove(player, ConnectedSocketState.Canceled);
                        }
                        else
                        {
                            Console.WriteLine("CheckCancelFromPlayers -> invalid json data");
                            Utils.QueueUserToRemove(player, ConnectedSocketState.InvalidData);
                        }
                    }
                    else if (socketError == SocketError.ConnectionReset)
                    {
                        Console.WriteLine("CheckCancelFromPlayers -> connected user fail");
                        Utils.QueueUserToRemove(player, ConnectedSocketState.ConnectionLost);
                    }
                }
            }

            RemoveInactiveUsers();
        }

        void RemoveInactiveUsers()
        {
            foreach (KeyValuePair<int, List<ConnectedPlayerData>> players in m_PendingPlayers)
            {
                players.Value.RemoveAll(userInfo =>
                    userInfo.state != ConnectedSocketState.WaitingForPlayers);
            }  
        }

        void CheckPendingUsersAfterConnectionError()
        {
            if(!m_HasPendingUsersAfterConnectionError)
            {
                return;
            }

            lock (ListenerLock)
            {
                foreach (ConnectedPlayerData player in m_PendingPlayersAfterConnectionError)
                {
                    AddNewConnectedUser(player.searchedPlayersAmount, player.socket);
                }

                m_PendingPlayersAfterConnectionError.Clear();

                m_HasPendingUsersAfterConnectionError = false;
            }
        }

        ////////////////////////////////////////////////////////////////////////
        
        bool m_IsActive = false;
        bool m_HasPendingUsersAfterConnectionError = false;

        TcpListener m_Listener = null;
        Dictionary<int, List<ConnectedPlayerData>> m_PendingPlayers = null;
        List<ConnectedPlayerData> m_PendingPlayersAfterConnectionError = null;

        OnPlayersConnected m_OnPlayersConnectedCallback = null;

        static readonly object ListenerLock = new object();
    }
}
