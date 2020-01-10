using System;
using System.Collections.Generic;
using System.Timers;
using System.Net.Sockets;
using System.Threading.Tasks;

using TBS_GameServer.Source.Utilities;
using static TBS_GameServer.Source.Network.NetworkHelper;

namespace TBS_GameServer.Source.Network
{
    public delegate void OnPlayersReady(List<ConnectedPlayerData> connectedPlayers);
    public delegate void OnRestartConnection(List<ConnectedPlayerData> connectedPlayers);

    class PlayersReadinessHandler
    {
        public void Init(List<ConnectedPlayerData> connectedPlayers, OnPlayersReady onPlayersReadyCallback, OnRestartConnection onRestartConnectionCallback)
        {
            m_ConnectedPlayersInProcess = connectedPlayers;
            m_ConnectedPlayersCount = m_ConnectedPlayersInProcess.Count;

            m_OnPlayersReadyCallback = onPlayersReadyCallback;
            m_OnRestartConnectionCallback = onRestartConnectionCallback;
        }
        public void Run()
        {
            Console.WriteLine("Waiting for players' readiness");

            m_ReadinessTimer = new Timer(NetworkDataConsts.ReadinessTimeMs);
            m_ReadinessTimer.Elapsed += delegate { m_IsTimerElapsed = true; };
            m_ReadinessTimer.Enabled = true;

            while (!m_IsTimerElapsed)
            {
                ProcessMessagesFromPlayers();

                RemoveInactiveUsers();

                if (m_ConnectedPlayersCount != m_ConnectedPlayersInProcess.Count)
                {
                    RestartPlayersConnection();
                    return;
                }

                CheckPlayersReadiness();
                if (m_ReadyPlayersCount == m_ConnectedPlayersCount)
                {
                    Console.WriteLine("Players are ready!");
                    Task.Run(() => m_OnPlayersReadyCallback(m_ConnectedPlayersInProcess));
                    return;
                }
            }

            FilterReadyPlayers();
            RestartPlayersConnection();
        }

        void DeactivateReadinessTimer()
        {
            m_ReadinessTimer.Enabled = false;
            m_ReadinessTimer = null;
        }

        void ProcessMessagesFromPlayers()
        {
            foreach (ConnectedPlayerData user in m_ConnectedPlayersInProcess)
            {
                byte[] buffer = new byte[NetworkDataConsts.DataSize];
                SocketError socketError;

                int receivesBytes = user.socket.Receive(buffer, 0, NetworkDataConsts.DataSize, SocketFlags.None, out socketError);
                if (receivesBytes > 0)
                {
                    string message;
                    if(TryProcessReadyMessage(buffer, out message))
                    {
                        if (message == NetworkDataConsts.IsReadyMessage && user.state != ConnectedSocketState.Ready)
                        {
                            user.state = ConnectedSocketState.Ready;
                            ++m_ReadyPlayersCount;
                            Console.WriteLine($"{m_ReadyPlayersCount.ToString()} ready");
                        }
                        else if (message == NetworkDataConsts.ClientCancelMessage)
                        {
                            Console.WriteLine("ProcessMessagesFromPlayers -> cancel from player");
                            QueueUserToRemove(user, ConnectedSocketState.Canceled);
                        }
                    }
                    else
                    {
                        Console.WriteLine("ProcessMessagesFromPlayers -> invalid json data");
                        QueueUserToRemove(user, ConnectedSocketState.InvalidData);
                    }
                }
                else if (socketError == SocketError.ConnectionReset)
                {
                    Console.WriteLine("ProcessMessagesFromPlayers -> connected user fail");
                    QueueUserToRemove(user, ConnectedSocketState.ConnectionLost);
                }
            }
        }

        void RemoveInactiveUsers()
        {
            m_ConnectedPlayersInProcess.RemoveAll(userInfo =>
            userInfo.state == ConnectedSocketState.Canceled
            || userInfo.state == ConnectedSocketState.ConnectionLost
            || userInfo.state == ConnectedSocketState.InvalidData);
        }

        void FilterReadyPlayers()
        {
            Console.WriteLine("OnReadinessTimeElapsed -> not all accepted");

            Dictionary<string, object> values = new Dictionary<string, object>();
            values.Add(NetworkDataConsts.MessageNameJsonKey, NetworkDataConsts.NotReadyMessage);
            byte[] buffer = Utils.JsonMessagePacker(values);

            foreach (ConnectedPlayerData user in m_ConnectedPlayersInProcess)
            {
                if (user.state == ConnectedSocketState.WaitingForPlayers)
                {
                    SocketError socketError;
                    user.socket.Send(buffer, 0, buffer.Length, SocketFlags.None, out socketError);
                    if (socketError == SocketError.ConnectionReset)
                    {
                        Console.WriteLine("OnReadinessTimeElapsed -> connected user fail");
                        QueueUserToRemove(user, ConnectedSocketState.ConnectionLost);
                    }
                }
            }

            m_ConnectedPlayersInProcess.RemoveAll(user => user.state != ConnectedSocketState.Ready);
        }

        void RestartPlayersConnection()
        {
            DeactivateReadinessTimer();

            Dictionary<string, object> values = new Dictionary<string, object>();
            values.Add(NetworkDataConsts.MessageNameJsonKey, NetworkDataConsts.WaitingForPlayersMessage);
            byte[] buffer = Utils.JsonMessagePacker(values);

            foreach (ConnectedPlayerData user in m_ConnectedPlayersInProcess)
            {
                user.state = ConnectedSocketState.WaitingForPlayers;

                SocketError socketError;
                user.socket.Send(buffer, 0, buffer.Length, SocketFlags.None, out socketError);
                if (socketError == SocketError.ConnectionReset)
                {
                    Console.WriteLine("RestartPlayersConnection -> connected user fail");
                    QueueUserToRemove(user, ConnectedSocketState.ConnectionLost);
                }
            }

            RemoveInactiveUsers();

            Console.WriteLine("Restarting players connection...");
            Task.Run(() => m_OnRestartConnectionCallback(m_ConnectedPlayersInProcess));
        }

        void CheckPlayersReadiness()
        {
            if (m_ReadyPlayersCount != m_ConnectedPlayersCount)
            {
                return;
            }

            Console.WriteLine("CheckPlayersReadiness -> checking...");
            DeactivateReadinessTimer();

            Dictionary<string, object> values = new Dictionary<string, object>();
            values.Add(NetworkDataConsts.MessageNameJsonKey, NetworkDataConsts.AllAreReadyMessage);
            byte[] buffer = Utils.JsonMessagePacker(values);

            foreach (ConnectedPlayerData user in m_ConnectedPlayersInProcess)
            {
                SocketError socketError;

                user.socket.Send(buffer, 0, buffer.Length, SocketFlags.None, out socketError);
                if (socketError == SocketError.ConnectionReset)
                {
                    Console.WriteLine("CheckPlayersReadiness -> connected user error");
                    QueueUserToRemove(user, ConnectedSocketState.ConnectionLost);
                }
            }

            m_ConnectedPlayersInProcess.RemoveAll(user => user.state != ConnectedSocketState.Ready);
            m_ReadyPlayersCount = m_ConnectedPlayersInProcess.Count;
        }

        bool m_IsTimerElapsed = false;
        int m_ReadyPlayersCount = 0;
        int m_ConnectedPlayersCount = 0;

        Timer m_ReadinessTimer = null;
        List<ConnectedPlayerData> m_ConnectedPlayersInProcess = null;

        OnPlayersReady m_OnPlayersReadyCallback = null;
        OnRestartConnection m_OnRestartConnectionCallback = null;
    }
}
