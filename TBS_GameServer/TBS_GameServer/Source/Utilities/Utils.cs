using System;
using System.Collections.Generic;
using System.Text.Json;
using System.Text;

namespace TBS_GameServer.Source.Utilities
{
    static class Utils
    {
        static public List<T> ShuffleList<T>(List<T> list)
        {
            Random rand = new Random();
            HashSet<int> shuffledIndeces = new HashSet<int>();
            while (shuffledIndeces.Count != list.Count)
            {
                shuffledIndeces.Add(rand.Next(0, list.Count));
            }

            List<T> toReturn = new List<T>();
            foreach (int index in shuffledIndeces)
            {
                toReturn.Add(list[index]);
            }

            return toReturn;
        }

        static public byte[] JsonSerialize<T>(T messageBody)
        {
            var message = new JsonNetMessage<T>();
            message.tbsGameMessage = messageBody;

            return Encoding.ASCII.GetBytes(JsonSerializer.Serialize(message));
        }

        static public T JsonDeserialize<T>(byte[] buffer)
        {
            var jsonMessage = new JsonNetMessage<T>();
            jsonMessage.tbsGameMessage = Activator.CreateInstance<T>();

            try
            {
                string jsonDoc = Encoding.UTF8.GetString(buffer, 0, buffer.Length).TrimEnd('\0');
                Console.WriteLine($"Deserialize -> {jsonDoc}");
                jsonMessage = JsonSerializer.Deserialize<JsonNetMessage<T>>(Encoding.UTF8.GetString(buffer, 0, buffer.Length).TrimEnd('\0'));

            }
            catch(JsonException)
            {
                Console.WriteLine($"{typeof(T)} -> Deserialize -> invalid doc");
            }

            return jsonMessage.tbsGameMessage;
        }

        static public string GetNextRoomId(string currentId)
        {
            if(LatinAlphabet == null)
            {
                LatinAlphabet = new List<char>();
                for(char c = 'A'; c <= 'Z'; ++c)
                {
                    LatinAlphabet.Add(c);
                }
            }

            string newId;
            if(currentId != null && currentId.Length > 0)
            {
                char lastChar = currentId[currentId.Length - 1];
                int nextCharIndex = LatinAlphabet.IndexOf(lastChar) + 1;

                if(nextCharIndex >= LatinAlphabet.Count)
                {
                    newId = currentId + LatinAlphabet[0].ToString();
                }
                else
                {
                    newId = LatinAlphabet[nextCharIndex].ToString();
                }
            }
            else
            {
                newId = LatinAlphabet[0].ToString();
            }

            return newId;
        }

        static List<char> LatinAlphabet = null;
    }
}
