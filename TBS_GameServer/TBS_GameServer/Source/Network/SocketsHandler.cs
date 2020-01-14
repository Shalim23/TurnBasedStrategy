using System;
using System.Collections.Generic;
using System.Net.Sockets;
using System.Threading.Tasks;

using TBS_GameServer.Source.Utilities;
using TBS_GameServer.Source.Events;

using static TBS_GameServer.Source.Events.Delegates;

namespace TBS_GameServer.Source.Network
{
    class SocketsHandler
    {
        public SocketsHandler(EventsManagerInstance eventsManager,
            List<ConnectedPlayerData> readyPlayers)
        {
            m_EventsManager = eventsManager;

            m_readyPlayers = new Dictionary<string, ConnectedPlayerData>();
            for (int index = 0; index < readyPlayers.Count; ++index)
            {
                m_readyPlayers.Add(JsonDataLoader.LoadedIds.Ids[index], readyPlayers[index]);
            }

            m_IsActive = true;
        }

        public void SendConnectionError()
        {
            Message message = new Message();
            message.messageName = NetworkDataConsts.TerminatingMessageName;

            byte[] buffer = Utils.JsonSerialize(message);

            foreach (KeyValuePair<string, ConnectedPlayerData> user in m_readyPlayers)
            {
                SocketError socketError;

                user.Value.socket.Send(buffer, 0, buffer.Length, SocketFlags.None, out socketError);
                user.Value.socket.Close();
            }
        }

        void InvokeNetMessage(string messageName, byte[] data)
        {
            NetworkMessageDelegate networkMessageDelegate;
            if (m_EventsManager.TryGetDelegate(DelegateType.NetworkMessage, out networkMessageDelegate))
            {
                Task.Run(() => networkMessageDelegate.Invoke(messageName, data));
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
                        Message message = Utils.JsonDeserialize<Message>(buffer);
                        if (message.IsValid())
                        {
                            InvokeNetMessage(message.messageName, buffer);
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
