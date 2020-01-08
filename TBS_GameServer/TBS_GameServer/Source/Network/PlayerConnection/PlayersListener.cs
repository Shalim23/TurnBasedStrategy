using System.Net.Sockets;
using System.Net;
using System;
using System.Collections.Generic;
using TBS_GameServer.Utilities;
using System.Text.Json;


namespace TBS_GameServer.Network.PlayerConnection
{
    class PlayersListener
    {
        public List<ConnectedPlayerData> WaitForPlayersConnection(List<ConnectedPlayerData> alreadyConnectedUsers)
        {
            Console.WriteLine($"Waiting for { NetworkDataConsts.MaxPlayers.ToString()} players connection");

            List<ConnectedPlayerData> connectedUsers = new List<ConnectedPlayerData>(alreadyConnectedUsers);
            m_Listener.Start();

            while (connectedUsers.Count != NetworkDataConsts.MaxPlayers)
            {
                Socket handler = TryAcceptUserConnection();
                if (handler != null)
                {
                    ConnectedPlayerData connectedPlayerData = new ConnectedPlayerData();
                    connectedPlayerData.state = ConnectedSocketState.WaitingForPlayers;
                    connectedPlayerData.socket = handler;

                    connectedUsers.Add(connectedPlayerData);
                    Console.WriteLine($"{connectedUsers.Count.ToString()} player(s) connected");
                }

                CheckCancelFromPlayers(connectedUsers);

                if (connectedUsers.Count == NetworkDataConsts.MaxPlayers)
                {
                    FinalizeConnection(connectedUsers);
                }
            }

            m_Listener.Stop();

            return connectedUsers;
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

            RemoveInactiveUsers(connectedUsers);
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

        bool IsCancelMessage(byte[] data)
        {
            JsonElement message;
            if(Utils.TryGetValidMessageJsonObject(data, out message))
            {
                JsonElement messageName;
                if (message.TryGetProperty(NetworkDataConsts.MessageNameJsonKey, out messageName)
                    && messageName.ValueKind == JsonValueKind.String
                    && messageName.GetString() == NetworkDataConsts.ClientCancelMessage)
                {
                    return true;
                }
            }

            return false;
        }

        void CheckCancelFromPlayers(List<ConnectedPlayerData> connectedUsers)
        {
            foreach (ConnectedPlayerData user in connectedUsers)
            {
                byte[] buffer = new byte[NetworkDataConsts.DataSize];
                SocketError socketError;

                int receivesBytes = user.socket.Receive(buffer, 0, NetworkDataConsts.DataSize, SocketFlags.None, out socketError);
                if (receivesBytes > 0)
                {
                    if (IsCancelMessage(buffer))
                    {
                        Console.WriteLine("CheckCancelFromPlayers -> cancel from player");
                        Utils.QueueUserToRemove(user, ConnectedSocketState.Canceled);
                    }
                    else
                    {
                        Console.WriteLine("CheckCancelFromPlayers -> invalid json data");
                        Utils.QueueUserToRemove(user, ConnectedSocketState.InvalidData);
                    }
                }
                else if (socketError == SocketError.ConnectionReset)
                {
                    Console.WriteLine("CheckCancelFromPlayers -> connected user fail");
                    Utils.QueueUserToRemove(user, ConnectedSocketState.ConnectionLost);
                }
            }

            RemoveInactiveUsers(connectedUsers);
        }

        private void RemoveInactiveUsers(List<ConnectedPlayerData> connectedUsers)
        {
            connectedUsers.RemoveAll(userInfo =>
            userInfo.state != ConnectedSocketState.WaitingForPlayers);
        }

        TcpListener m_Listener = new TcpListener(IPAddress.Parse(NetworkDataConsts.Ip), NetworkDataConsts.Port);
    }
}
