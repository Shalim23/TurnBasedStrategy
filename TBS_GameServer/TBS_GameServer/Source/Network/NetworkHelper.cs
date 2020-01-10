using System;
using System.Text.Json;

using TBS_GameServer.Source.Utilities;

namespace TBS_GameServer.Source.Network
{
    static class NetworkHelper
    {
        static public void QueueUserToRemove(ConnectedPlayerData connectedPlayerData, ConnectedSocketState newState)
        {
            connectedPlayerData.state = newState;
            connectedPlayerData.socket.Close();
        }

        static public bool TryParseNewConnectedUserMessage(byte[] buffer, out int searchedPlayerAmount)
        {
            searchedPlayerAmount = 0;

            JsonElement message;
            if (!Utils.TryGetValidMessageJsonObject(buffer, out message))
            {
                Console.WriteLine("ProcessNewConnectedUser -> invalid data was sent on connection");
                return false;
            }

            JsonElement messageName;
            if (!message.TryGetProperty(NetworkDataConsts.MessageNameJsonKey, out messageName)
            || messageName.ValueKind != JsonValueKind.String
            || messageName.GetString() != NetworkDataConsts.ClientConnectionMessage)
            {
                Console.WriteLine("ProcessNewConnectedUser -> invalid message was sent on connection");
                return false;
            }

            JsonElement playersAmountEntry;
            if (message.TryGetProperty(NetworkDataConsts.PlayersAmountJsonKey, out playersAmountEntry)
                && playersAmountEntry.ValueKind == JsonValueKind.Number)
            {
                int playersAmount = playersAmountEntry.GetInt32();
                //#TODO change to MinPlayers constant after debug
                if (playersAmount > 0 && playersAmount <= NetworkDataConsts.MaxPlayers)
                {
                    searchedPlayerAmount = playersAmount;
                    return true;
                }
                else
                {
                    Console.WriteLine("ProcessNewConnectedUser -> invalid PlayersAmount was sent on connection");
                    return false;
                }
            }
            else
            {
                Console.WriteLine("ProcessNewConnectedUser -> invalid PlayersAmount entry was sent on connection");
                return false;
            }
        }
        static public bool TryParseCancelMessage(byte[] data)
        {
            JsonElement message;
            if (Utils.TryGetValidMessageJsonObject(data, out message))
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

        static public bool TryProcessReadyMessage(byte[] data, out string messageNameValue)
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
    }
}
