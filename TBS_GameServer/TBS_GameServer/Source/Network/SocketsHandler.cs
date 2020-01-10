using System;
using System.Collections.Generic;
using System.Net.Sockets;
using System.Text.Json;
using System.Threading.Tasks;

using TBS_GameServer.Source.Utilities;
using TBS_GameServer.Source.Events;

using static TBS_GameServer.Source.Events.Delegates;

namespace TBS_GameServer.Source.Network
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

        void InvokeNetMessage(JsonElement message)
        {
            NetworkMessageDelegate networkMessageDelegate;
            if (m_EventsManager.TryGetDelegate(DelegateType.NetworkMessage, out networkMessageDelegate))
            {
                Task.Run(() => networkMessageDelegate.Invoke(message));
            }
            else
            {
                Console.Write($"{DelegateType.ConnectionError.ToString()} was not invoked");
            }
        }

        public void ProcessReceive()
        {
            while(m_IsActive)
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
                            InvokeNetMessage(message);
                        }
                        else
                        {
                            Console.WriteLine("ProcessReceive -> invalid json data");
                            ProcessConnectionError(user.Value);
                            break;
                        }
                    }
                    else if (socketError == SocketError.ConnectionReset)
                    {
                        Console.WriteLine("ProcessReceive -> connected user fail");
                        ProcessConnectionError(user.Value);
                        break;
                    }
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

            m_IsActive = true;
        }

        void ProcessConnectionError(ConnectedPlayerData user)
        {
            m_IsActive = false;
            user.socket.Close();

            ConnectionErrorDelegate connectionErrorDelegate;
            if (m_EventsManager.TryGetDelegate(DelegateType.ConnectionError, out connectionErrorDelegate))
            {
                Task.Run(() => connectionErrorDelegate.Invoke());
            }
            else
            {
                Console.Write($"{DelegateType.ConnectionError.ToString()} was not invoked");
            }
        }

        EventsManagerInstance m_EventsManager = null;

        Dictionary<string, ConnectedPlayerData> m_readyPlayers = null;
        bool m_IsActive = false;
    }
}
