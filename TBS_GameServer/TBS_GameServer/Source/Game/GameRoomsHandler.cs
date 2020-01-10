using System;
using System.Collections.Generic;
using System.Threading.Tasks;

using TBS_GameServer.Source.Network;
using TBS_GameServer.Source.Utilities;


namespace TBS_GameServer.Source.Game
{
    class GameRoomsHandler
    {
        
        public void Init()
        {

        }
        public void CreateRoom(List<ConnectedPlayerData> connectedPlayers)
        {
            m_NextRoomId = Utils.GetNextRoomId(m_NextRoomId);

            GameRoom newRoom = new GameRoom();
            newRoom.Init(m_NextRoomId, connectedPlayers, roomId => OnRoomShutdown(roomId));

            lock (GameRoomsLock)
            {
                m_GameRoomsById.Add(m_NextRoomId, newRoom);
                Console.WriteLine($"CreateRoom -> {m_NextRoomId} was created.");

                Console.WriteLine($"CreateRoom -> {m_NextRoomId} is starting.");
                Task.Run(m_GameRoomsById[m_NextRoomId].Run);
            }
        }

        void OnRoomShutdown(string roomId)
        {
            lock(GameRoomsLock)
            {
                if(!m_GameRoomsById.Remove(roomId))
                {
                    Console.WriteLine($"OnRoomShutdown -> {roomId} was shut down!");
                }
                else
                {
                    Console.WriteLine($"OnRoomShutdown -> {roomId} was not found!!!");
                }
            }
        }

        string m_NextRoomId = null;
        Dictionary<string, GameRoom> m_GameRoomsById = new Dictionary<string, GameRoom>();

        static readonly object GameRoomsLock = new object();
    }
}
