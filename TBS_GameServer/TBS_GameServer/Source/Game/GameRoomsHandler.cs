using System;
using System.Collections.Generic;
using System.Text;
using TBS_GameServer.Network;

namespace TBS_GameServer.Source.Game
{
    class GameRoomsHandler
    {
        //#TODO need lock for adding and cleaning rooms
        public void CreateRoom(List<ConnectedPlayerData> connectedPlayers)
        {

        }

        static int RoomId = 0;
        Dictionary<int, GameRoom> m_GameRooms;
    }
}
