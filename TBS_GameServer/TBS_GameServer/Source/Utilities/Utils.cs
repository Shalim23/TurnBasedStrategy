using System;
using System.Collections.Generic;
using System.Text.Json;
using System.Text;
using System.IO;
using TBS_GameServer.Source.Network;

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

        static public bool TryParseJson(byte[] data, out JsonDocument jsonDocument)
        {
            try
            {
                jsonDocument = JsonDocument.Parse(Encoding.UTF8.GetString(data).TrimEnd('\0'));
            }
            catch(JsonException)
            {
                jsonDocument = null;
                return false;
            }

            return true;
        }

        static public bool TryGetValidMessageJsonObject(byte[] data, out JsonElement element)
        {
            JsonDocument doc;
            if (TryParseJson(data, out doc))
            {
                if (doc.RootElement.TryGetProperty(NetworkDataConsts.TBSGameMessageJsonKey, out element))
                {
                    return true;
                }
            }

            element = new JsonElement(); //cannot be null
            return false;
        }

        static public byte[] JsonMessagePacker(Dictionary<string, object> values)
        {
            MemoryStream memoryStream = new MemoryStream();
            Utf8JsonWriter utf8JsonWriter = new Utf8JsonWriter(memoryStream);

            utf8JsonWriter.WriteStartObject();

            utf8JsonWriter.WriteStartObject(NetworkDataConsts.TBSGameMessageJsonKey);

            foreach (KeyValuePair<string, object> value in values)
            {
                utf8JsonWriter.WritePropertyName(value.Key);

                Type valueType = value.Value.GetType();
                if(valueType == typeof(int))
                {
                    utf8JsonWriter.WriteNumberValue(Convert.ToInt32(value.Value));
                }
                else if(valueType == typeof(string))
                {
                    utf8JsonWriter.WriteStringValue(Convert.ToString(value.Value));
                }
                else if(valueType == typeof(bool))
                {
                    utf8JsonWriter.WriteBooleanValue(Convert.ToBoolean(value.Value));
                }
                else
                {
                    utf8JsonWriter.WriteNullValue();
                    Console.WriteLine($"JsonMessagePacker -> unsuported type {value.Key} - {value.Value}");
                }
            }

            utf8JsonWriter.WriteEndObject();

            utf8JsonWriter.WriteEndObject();
            utf8JsonWriter.Flush();

            memoryStream.Position = 0;
            StreamReader reader = new StreamReader(memoryStream);

            return Encoding.ASCII.GetBytes(reader.ReadToEnd());
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
