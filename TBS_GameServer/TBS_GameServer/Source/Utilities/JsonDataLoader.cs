using System;
using System.Collections.Generic;
using System.IO;
using System.Text.Json;

namespace TBS_GameServer.Source.Utilities
{
    static class LoadableData
    {
        public static List<string> Ids;
    }

    static class JsonDataLoader
    {
        public static void LoadJsonData()
        {
            Console.WriteLine("Loading Json data...");
            LoadIds();
        }

        static void LoadIds()
        {
            List<string> ids = new List<string>();
            string path = Environment.CurrentDirectory + @"\JsonData\IdNames.json";
            string text = File.ReadAllText(path);

            JsonDocument doc = JsonDocument.Parse(text);
            JsonElement idsObject = doc.RootElement.GetProperty("Ids");
            foreach (JsonElement id in idsObject.EnumerateArray())
            {
                if (id.ValueKind == JsonValueKind.String)
                {
                    ids.Add(id.GetString());
                }
            }

            LoadableData.Ids = ids;
            Console.WriteLine("Ids loaded...");
        }
    }
}
