using System;
using System.Collections.Generic;
using System.IO;
using System.Text.Json;
using TBS_GameServer.Game.GameData;

namespace TBS_GameServer.Utilities
{
    static class LoadableData
    {
        //static public List<string> Ids;
    }

    static class JsonDataLoader
    {
        public static void LoadJsonData()
        {
            LoadIds();
        }

        static void LoadIds()
        {
            //List<string> ids = new List<string>();
            //string path = Environment.CurrentDirectory + @"\JsonData\IdNames.json";
            //string text = File.ReadAllText(path);

            //JsonDocument doc = JsonDocument.Parse(text);
            //JsonElement idsObject = doc.RootElement.GetProperty("Ids");
            //foreach(JsonElement id in idsObject.EnumerateArray())
            //{
            //    if(id.ValueKind == JsonValueKind.String)
            //    {
            //        ids.Add(id.GetString());
            //    }
            //}

            //LoadableData.Ids = ids;
            //Console.WriteLine("Ids initialized");
        }

        //static void LoadDistrictsCardsData()
        //{
        //    List<GameDataStructs.District> districts = new List<GameDataStructs.District>();
        //    string path = Environment.CurrentDirectory + @"\JsonData\DistrictCards.json";
        //    string text = File.ReadAllText(path);

        //    JsonDocument doc = JsonDocument.Parse(text);
            
        //    foreach (JsonProperty docProperty in doc.RootElement.EnumerateObject())
        //    {
        //        string color = null;
        //        int price = 0;
        //        int amount = 0;

        //        foreach (JsonProperty property in docProperty.Value.EnumerateObject())
        //        {
        //            if (property.Name == "color" && property.Value.ValueKind == JsonValueKind.String)
        //            {
        //                color = property.Value.GetString();
        //            }
        //            else if(property.Name == "price" && property.Value.ValueKind == JsonValueKind.Number)
        //            {
        //                price = property.Value.GetUInt16();
        //            }
        //            else if(property.Name == "amount" && property.Value.ValueKind == JsonValueKind.Number)
        //            {
        //                amount = property.Value.GetUInt16();
        //            }
        //        }

        //        for (int i = 0; i < amount; ++i)
        //        {
        //            GameDataStructs.District district = new GameDataStructs.District();
        //            district.name = docProperty.Name;
        //            district.price = price;
        //            district.color = color;

        //            districts.Add(district);
        //        }
        //    }

        //    LoadableData.Districts = districts;
        //    Console.WriteLine("Districts initialized");
        //}

        //static void LoadCharactersCardsData()
        //{
        //    List<GameDataStructs.Character> characters = new List<GameDataStructs.Character>();

        //    string path = Environment.CurrentDirectory + @"\JsonData\CharacterCards.json";
        //    string text = File.ReadAllText(path);

        //    JsonDocument doc = JsonDocument.Parse(text);
        //    foreach (JsonProperty docProperty in doc.RootElement.EnumerateObject())
        //    {
        //        foreach (JsonProperty property in docProperty.Value.EnumerateObject())
        //        {
        //            if (property.Name == "hasActiveSkill"
        //                && (property.Value.ValueKind == JsonValueKind.True || property.Value.ValueKind == JsonValueKind.False))
        //            {
        //                GameDataStructs.Character character = new GameDataStructs.Character();
        //                character.name = docProperty.Name;
        //                character.hasActiveSkill = property.Value.GetBoolean();

        //                characters.Add(character);
        //            }
        //        }
        //    }

        //    LoadableData.Characters = characters;
        //    Console.WriteLine("Characters initialized");
        //}
    }
}
