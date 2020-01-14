using System;
using System.Collections.Generic;
using System.IO;
using System.Text.Json;

namespace TBS_GameServer.Source.Utilities
{
    static class JsonDataLoader
    {
        public static void LoadJsonData()
        {
            Console.WriteLine("Loading Json data...");
            LoadIds();
        }

        static void LoadIds()
        {
            string path = Environment.CurrentDirectory + @"\JsonData\IdNames.json";
            string text = File.ReadAllText(path);

            LoadedIds = new JsonLoadableDataIds();
            LoadedIds = JsonSerializer.Deserialize<JsonLoadableDataIds>(text);
            Console.WriteLine("Ids loaded...");
        }

        public static JsonLoadableDataIds LoadedIds = null;
    }

    class JsonLoadableDataIds
    {
        public JsonLoadableDataIds()
        {
            Ids = new List<string>();
        }

        public List<string> Ids { get; set; }
    }
}
