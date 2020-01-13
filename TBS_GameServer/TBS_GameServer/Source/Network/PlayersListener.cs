using System.Net.Sockets;
using System.Net;
using System;
using System.Text;
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
            m_NewConnectedUsers = new Dictionary<DateTime, Socket>();

            m_OnPlayersConnectedCallback = onPlayersConnectedCallback;

            m_IsActive = true;

            m_Listener.Start();
            Console.WriteLine("Started listening...");
        }

        public void Run()
        {
            while (m_IsActive)
            {
                CheckIncomingConnections();
                CheckNewConnectedUserMessage();
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

        void CheckIncomingConnections()
        {
            Socket handler = TryAcceptUserConnection();
            if (handler != null)
            {
                m_NewConnectedUsers.Add(DateTime.Now, handler);
            }
        }

        void AddNewConnectedUser(int searchedPlayerAmount, Socket handler)
        {
            //#TODO remove after debugging
            if(searchedPlayerAmount == 1)
            {
                ConnectedPlayerData connectedPlayerData = new ConnectedPlayerData();
                connectedPlayerData.state = ConnectedSocketState.WaitingForPlayers;
                connectedPlayerData.socket = handler;
                connectedPlayerData.searchedPlayersAmount = searchedPlayerAmount;

                List<ConnectedPlayerData> temp = new List<ConnectedPlayerData>();
                temp.Add(connectedPlayerData);
                FinalizeConnection(temp);

                Task.Run(() => m_OnPlayersConnectedCallback(temp));
                return;
            }
            /////////////////////////////////////////////////////////////////////////

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
                Console.WriteLine($"{newPendingPlayerForSearchedAmount.Count.ToString()} player(s) for {searchedPlayerAmount} players room");

                m_PendingPlayers.Add(searchedPlayerAmount, newPendingPlayerForSearchedAmount);
            }
        }

        void CheckNewConnectedUserMessage()
        {
            TimeSpan timeLimit = TimeSpan.FromSeconds(NetworkDataConsts.NewConnectedUserMessageTimeLimit);
            List<DateTime> usersToRemove = new List<DateTime>();

            foreach(KeyValuePair<DateTime, Socket> newUser in m_NewConnectedUsers)
            {
                byte[] buffer = new byte[NetworkDataConsts.DataSize];
                SocketError socketError;

                if (newUser.Value.Receive(buffer, 0, NetworkDataConsts.DataSize, SocketFlags.None, out socketError) > 0)
                {
                    if(TryProcessNewConnectedUser(buffer, newUser.Value))
                    {
                        usersToRemove.Add(newUser.Key);
                    }
                }
                else if(DateTime.Now - newUser.Key > timeLimit)
                {
                    Console.WriteLine("CheckNewConnectedUserMessage -> no data was sent on connection");
                    usersToRemove.Add(newUser.Key);
                }
            }

            foreach(DateTime time in usersToRemove)
            {
                m_NewConnectedUsers.Remove(time);
            }
        }

        bool TryProcessNewConnectedUser(byte[] buffer, Socket socket)
        {
            ClientConnectionMessage clientConnectionMessage = new ClientConnectionMessage();
            
            if(Utils.TryJsonDeserialize(buffer, out clientConnectionMessage) 
                && clientConnectionMessage.messageName == NetworkDataConsts.ClientConnectionMessageName
                && clientConnectionMessage.playersAmount != null)
            {
                AddNewConnectedUser((int)clientConnectionMessage.playersAmount, socket);
                return true;
            }

            Console.WriteLine($"TryProcessNewConnectedUser -> invalid data\n{Encoding.UTF8.GetString(buffer, 0, buffer.Length).TrimEnd('\0')}");
            return false;
        }
        
        private void FinalizeConnection(List<ConnectedPlayerData> connectedUsers)
        {
            Message message = new Message();
            message.messageName = NetworkDataConsts.WaitingForReadinessMessageName;
            
            byte[] buffer = Utils.JsonSerialize(message);
            
            foreach (ConnectedPlayerData user in connectedUsers)
            {
                SocketError socketError;

                user.socket.Send(buffer, 0, buffer.Length, SocketFlags.None, out socketError);
                if (socketError == SocketError.ConnectionReset)
                {
                    Console.WriteLine("WaitForUserConnection -> connected user fail");
                    QueueUserToRemove(user, ConnectedSocketState.ConnectionLost);
                }
            }

            RemoveInactiveUsers();
        }

        Socket TryAcceptUserConnection()
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
                        Message message = new Message();
                        
                        if (Utils.TryJsonDeserialize(buffer, out message) 
                            && message.messageName == NetworkDataConsts.ClientCancelMessageName)
                        {
                            Console.WriteLine("CheckCancelFromPlayers -> cancel from player");
                            QueueUserToRemove(player, ConnectedSocketState.Canceled);
                        }
                        else
                        {
                            Console.WriteLine("CheckCancelFromPlayers -> invalid json data");
                            QueueUserToRemove(player, ConnectedSocketState.InvalidData);
                        }
                    }
                    else if (socketError == SocketError.ConnectionReset)
                    {
                        Console.WriteLine("CheckCancelFromPlayers -> connected user fail");
                        QueueUserToRemove(player, ConnectedSocketState.ConnectionLost);
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

        Dictionary<DateTime, Socket> m_NewConnectedUsers = null;
        Dictionary<int, List<ConnectedPlayerData>> m_PendingPlayers = null;
        List<ConnectedPlayerData> m_PendingPlayersAfterConnectionError = null;

        OnPlayersConnected m_OnPlayersConnectedCallback = null;

        static readonly object ListenerLock = new object();
    }
}
