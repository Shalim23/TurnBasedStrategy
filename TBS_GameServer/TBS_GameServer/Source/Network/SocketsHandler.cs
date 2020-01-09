using System;
using System.Collections.Generic;
using System.Net.Sockets;
using TBS_GameServer.Utilities;
using System.Text.Json;
using TBS_GameServer.Events;

using static TBS_GameServer.Events.Delegates;

namespace TBS_GameServer.Network
{
    class SocketsHandler
    { 
        public void SendConnectionError()
        {
            Dictionary<string, object> values = new Dictionary<string, object>();
            values.Add(NetworkDataConsts.MessageNameJsonKey, NetworkDataConsts.TerminatingMessage);
            byte[] buffer = Utils.JsonMessagePacker(values);

            foreach (KeyValuePair<string, ConnectedPlayerData> user in m_readyPlayers)
            {
                SocketError socketError;

                user.Value.socket.Send(buffer, 0, buffer.Length, SocketFlags.None, out socketError);
                user.Value.socket.Close();
            }
        }

        public void ProcessReceive()
        {
            foreach (KeyValuePair<string, ConnectedPlayerData> user in m_readyPlayers)
            {
                byte[] buffer = new byte[NetworkDataConsts.DataSize];
                SocketError socketError;

                int receivesBytes = user.Value.socket.Receive(buffer, 0, NetworkDataConsts.DataSize, SocketFlags.None, out socketError);
                if (receivesBytes > 0)
                {
                    JsonElement message;
                    if (Utils.TryGetValidMessageJsonObject(buffer, out message))
                    {
                        NetworkMessageDelegate networkMessageDelegate;
                        if (m_EventsManager.TryGetDelegate(DelegateType.NetworkMessage, out networkMessageDelegate))
                        {
                            networkMessageDelegate.Invoke(message);
                        }
                        else
                        {
                            Console.Write($"{DelegateType.ConnectionError.ToString()} was not invoked");
                        }
                    }
                    else
                    {
                        Console.WriteLine("ProcessReceive -> invalid json data");
                        ProcessConnectionError(user.Value);
                    }
                }
                else if (socketError == SocketError.ConnectionReset)
                {
                    Console.WriteLine("ProcessReceive -> connected user fail");
                    ProcessConnectionError(user.Value);
                }
            }
        }

        public void Init(EventsManagerInstance eventsManager, List<ConnectedPlayerData> readyPlayers)
        {
            m_EventsManager = eventsManager;
            m_readyPlayers = new Dictionary<string, ConnectedPlayerData>();
            for (int index = 0; index < readyPlayers.Count; ++index)
            {
                m_readyPlayers.Add(LoadableData.Ids[index], readyPlayers[index]);
            }

            PlayersConnectedDelegate playersConnectedDelegate;
            if (m_EventsManager.TryGetDelegate(DelegateType.PlayersConnected, out playersConnectedDelegate))
            {
                playersConnectedDelegate.Invoke();
            }
            else
            {
                Console.Write($"{DelegateType.PlayersConnected.ToString()} was not invoked");
            }
        }

        void ProcessConnectionError(ConnectedPlayerData user)
        {
            user.socket.Close();
            ConnectionErrorDelegate connectionErrorDelegate;
            if (m_EventsManager.TryGetDelegate(DelegateType.ConnectionError, out connectionErrorDelegate))
            {
                connectionErrorDelegate.Invoke();
            }
            else
            {
                Console.Write($"{DelegateType.ConnectionError.ToString()} was not invoked");
            }
        }

        EventsManagerInstance m_EventsManager = null;
        Dictionary<string, ConnectedPlayerData> m_readyPlayers;
    }
}
