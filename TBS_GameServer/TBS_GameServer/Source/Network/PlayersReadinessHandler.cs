using System;
using System.Collections.Generic;
using System.Timers;
using System.Net.Sockets;
using TBS_GameServer.Source.Utilities;
using System.Text.Json;

namespace TBS_GameServer.Source.Network
{
    class PlayersReadinessHandler
    {
        public List<ConnectedPlayerData> WaitForPlayersReadiness(List<ConnectedPlayerData> connectedPlayers)
        {
            List<ConnectedPlayerData> pendingPlayers = new List<ConnectedPlayerData>(connectedPlayers);
            Console.WriteLine("Waiting for players' readiness");

            ResetData();
            ActivateReadinessTimer();

            while (!m_IsTimerElapsed)
            {
                ProcessMessagesFromPlayers(pendingPlayers);

                RemoveInactiveUsers(pendingPlayers);

                if (pendingPlayers.Count != connectedPlayers.Count)
                {
                    RestartPlayersConnection(pendingPlayers);
                    break;
                }

                CheckPlayersReadiness(pendingPlayers);
                if (m_ReadyPlayersCount == connectedPlayers.Count)
                {
                    Console.WriteLine("Players are ready!");
                    break;
                }
            }

            if (m_IsTimerElapsed)
            {
                FilterReadyPlayers(pendingPlayers);
                RestartPlayersConnection(pendingPlayers);
            }

            return pendingPlayers;
        }

        private void ResetData()
        {
            m_IsTimerElapsed = false;
            m_ReadyPlayersCount = 0;
        }

        private void ActivateReadinessTimer()
        {
            m_ReadinessTimer = new Timer(NetworkDataConsts.ReadinessTimeMs);
            m_ReadinessTimer.Elapsed += delegate { m_IsTimerElapsed = true; };
            m_ReadinessTimer.Enabled = true;
        }

        private void DeactivateReadinessTimer()
        {
            m_ReadinessTimer.Enabled = false;
            m_ReadinessTimer = null;
        }

        bool TryProcessMessage(byte[] data, out string messageNameValue)
        {
            JsonElement message;
            if (Utils.TryGetValidMessageJsonObject(data, out message))
            {
                JsonElement messageName;
                if (message.TryGetProperty(NetworkDataConsts.MessageNameJsonKey, out messageName)
                    && messageName.ValueKind == JsonValueKind.String)
                {
                    messageNameValue = messageName.GetString();
                    return true;
                }
            }

            messageNameValue = "";
            return false;
        }

        private void ProcessMessagesFromPlayers(List<ConnectedPlayerData> pendingPlayers)
        {
            foreach (ConnectedPlayerData user in pendingPlayers)
            {
                byte[] buffer = new byte[NetworkDataConsts.DataSize];
                SocketError socketError;

                int receivesBytes = user.socket.Receive(buffer, 0, NetworkDataConsts.DataSize, SocketFlags.None, out socketError);
                if (receivesBytes > 0)
                {
                    string message;
                    if(TryProcessMessage(buffer, out message))
                    {
                        if (message == NetworkDataConsts.IsReadyMessage
                        && user.state != ConnectedSocketState.Ready)
                        {
                            user.state = ConnectedSocketState.Ready;
                            ++m_ReadyPlayersCount;
                            Console.WriteLine($"{m_ReadyPlayersCount.ToString()} ready");
                        }
                        else if (message == NetworkDataConsts.ClientCancelMessage)
                        {
                            Console.WriteLine("ProcessMessagesFromPlayers -> cancel from player");
                            Utils.QueueUserToRemove(user, ConnectedSocketState.Canceled);
                        }
                    }
                    else
                    {
                        Console.WriteLine("ProcessMessagesFromPlayers -> invalid json data");
                        Utils.QueueUserToRemove(user, ConnectedSocketState.InvalidData);
                    }
                }
                else if (socketError == SocketError.ConnectionReset)
                {
                    Console.WriteLine("ProcessMessagesFromPlayers -> connected user fail");
                    Utils.QueueUserToRemove(user, ConnectedSocketState.ConnectionLost);
                }
            }
        }

        private void RemoveInactiveUsers(List<ConnectedPlayerData> pendingPlayers)
        {
            pendingPlayers.RemoveAll(userInfo =>
            userInfo.state == ConnectedSocketState.Canceled
            || userInfo.state == ConnectedSocketState.ConnectionLost
            || userInfo.state == ConnectedSocketState.InvalidData);
        }

        private void FilterReadyPlayers(List<ConnectedPlayerData> pendingPlayers)
        {
            Console.WriteLine("OnReadinessTimeElapsed -> not all accepted");

            Dictionary<string, object> values = new Dictionary<string, object>();
            values.Add(NetworkDataConsts.MessageNameJsonKey, NetworkDataConsts.NotReadyMessage);
            byte[] buffer = Utils.JsonMessagePacker(values);

            foreach (ConnectedPlayerData user in pendingPlayers)
            {
                if (user.state == ConnectedSocketState.WaitingForPlayers)
                {
                    SocketError socketError;
                    user.socket.Send(buffer, 0, buffer.Length, SocketFlags.None, out socketError);
                    if (socketError == SocketError.ConnectionReset)
                    {
                        Console.WriteLine("OnReadinessTimeElapsed -> connected user fail");
                        Utils.QueueUserToRemove(user, ConnectedSocketState.ConnectionLost);
                    }
                }
            }

            pendingPlayers.RemoveAll(user => user.state != ConnectedSocketState.Ready);
        }

        private void RestartPlayersConnection(List<ConnectedPlayerData> pendingPlayers)
        {
            DeactivateReadinessTimer();

            Dictionary<string, object> values = new Dictionary<string, object>();
            values.Add(NetworkDataConsts.MessageNameJsonKey, NetworkDataConsts.WaitingForPlayersMessage);
            byte[] buffer = Utils.JsonMessagePacker(values);

            foreach (ConnectedPlayerData user in pendingPlayers)
            {
                user.state = ConnectedSocketState.WaitingForPlayers;

                SocketError socketError;
                user.socket.Send(buffer, 0, buffer.Length, SocketFlags.None, out socketError);
                if (socketError == SocketError.ConnectionReset)
                {
                    Console.WriteLine("RestartPlayersConnection -> connected user fail");
                    Utils.QueueUserToRemove(user, ConnectedSocketState.ConnectionLost);
                }
            }

            RemoveInactiveUsers(pendingPlayers);

            Console.WriteLine("Restarting players connection...");
        }

        private void CheckPlayersReadiness(List<ConnectedPlayerData> pendingPlayers)
        {
            if (m_ReadyPlayersCount != pendingPlayers.Count)
            {
                return;
            }

            Console.WriteLine("CheckPlayersReadiness -> checking...");
            DeactivateReadinessTimer();

            Dictionary<string, object> values = new Dictionary<string, object>();
            values.Add(NetworkDataConsts.MessageNameJsonKey, NetworkDataConsts.AllAreReadyMessage);
            byte[] buffer = Utils.JsonMessagePacker(values);

            foreach (ConnectedPlayerData user in pendingPlayers)
            {
                SocketError socketError;

                user.socket.Send(buffer, 0, buffer.Length, SocketFlags.None, out socketError);
                if (socketError == SocketError.ConnectionReset)
                {
                    Console.WriteLine("CheckPlayersReadiness -> connected user error");
                    Utils.QueueUserToRemove(user, ConnectedSocketState.ConnectionLost);
                }
            }

            pendingPlayers.RemoveAll(user => user.state != ConnectedSocketState.Ready);
            m_ReadyPlayersCount = pendingPlayers.Count;
        }

        bool m_IsTimerElapsed = false;
        int m_ReadyPlayersCount = 0;
        Timer m_ReadinessTimer = null;
    }
}
