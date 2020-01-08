using System;
using System.Collections.Generic;
using System.Net.Sockets;
using TBS_GameServer.Utilities;
using System.Text.Json;
using TBS_GameServer.Events;

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
                        Delegates.NetworkMessageDelegate networkMessageDelegate;
                        if (EventsManager.GetInstance().TryGetDelegate(Delegates.DelegateType.NetworkMessage, out networkMessageDelegate))
                        {
                            networkMessageDelegate.Invoke(message);
                        }
                        else
                        {
                            Console.Write($"{Delegates.DelegateType.ConnectionError.ToString()} was not invoked");
                        }
                    }
                    else
                    {
                        Console.WriteLine("CheckCancelFromPlayers -> invalid json data");
                        ProcessConnectionError(user.Value);
                    }
                }
                else if (socketError == SocketError.ConnectionReset)
                {
                    Console.WriteLine("SocketsHandler.Receive -> connected user fail");
                    ProcessConnectionError(user.Value);
                }
            }
        }

        public void Activate(List<ConnectedPlayerData> readyPlayers)
        {
            m_readyPlayers = new Dictionary<string, ConnectedPlayerData>();
            for (int index = 0; index < readyPlayers.Count; ++index)
            {
                m_readyPlayers.Add(LoadableData.Ids[index], readyPlayers[index]);
            }

            Delegates.PlayersConnectedDelegate playersConnectedDelegate;
            if (EventsManager.GetInstance().TryGetDelegate(Delegates.DelegateType.PlayersConnected, out playersConnectedDelegate))
            {
                playersConnectedDelegate.Invoke(m_readyPlayers.Count);
            }
            else
            {
                Console.Write($"{Delegates.DelegateType.PlayersConnected.ToString()} was not invoked");
            }
        }

        void ProcessConnectionError(ConnectedPlayerData user)
        {
            user.socket.Close();
            Delegates.ConnectionErrorDelegate connectionErrorDelegate;
            if (EventsManager.GetInstance().TryGetDelegate(Delegates.DelegateType.ConnectionError, out connectionErrorDelegate))
            {
                connectionErrorDelegate.Invoke();
            }
            else
            {
                Console.Write($"{Delegates.DelegateType.ConnectionError.ToString()} was not invoked");
            }
        }

        Dictionary<string, ConnectedPlayerData> m_readyPlayers;
    }
}
