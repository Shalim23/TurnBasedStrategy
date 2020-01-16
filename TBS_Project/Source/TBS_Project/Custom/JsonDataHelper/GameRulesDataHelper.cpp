#include "GameRulesDataHelper.h"
#include "Misc/Paths.h"
#include "Misc/FileHelper.h"
#include "Serialization/JsonSerializer.h"

void GameRulesDataHelper::InitGameRulesData()
{
    const FString& JsonFilePath = FPaths::ProjectContentDir() + "/JsonData/GameRules.json";
    FString jsonString;
    FFileHelper::LoadFileToString(jsonString, *JsonFilePath);

    TSharedPtr<FJsonObject> gameRulesCategoriesDataJson = MakeShareable(new FJsonObject());
    TSharedRef<TJsonReader<>> jsonReader = TJsonReaderFactory<>::Create(jsonString);

    if (FJsonSerializer::Deserialize(jsonReader, gameRulesCategoriesDataJson)
        && gameRulesCategoriesDataJson.IsValid())
    {
        m_Data.reserve(gameRulesCategoriesDataJson->Values.Num());
        for (const auto& gameRulesCategories : gameRulesCategoriesDataJson->Values)
        {
            GameRulesCategoryData gameRulesCategoryDataToPush;
            const FString& gameRulesCategoriesKey = gameRulesCategories.Key;
            gameRulesCategoryDataToPush.header = gameRulesCategoriesKey;

            const TSharedPtr<FJsonObject>* gameRulesCurrentCategoryJsonPointer = nullptr;
            if (gameRulesCategoriesDataJson->TryGetObjectField(gameRulesCategoriesKey, gameRulesCurrentCategoryJsonPointer))
            {
                gameRulesCategoryDataToPush.gameRulesData =
                    ProcessGameRulesCategory(*gameRulesCurrentCategoryJsonPointer);
            }

            m_Data.push_back(gameRulesCategoryDataToPush);
        }
    }
}

void GameRulesDataHelper::Clear()
{
    m_Data.clear();
}

std::vector<GameRulesData> GameRulesDataHelper::ProcessGameRulesCategory(const TSharedPtr<FJsonObject>& gameRulesCurrentCategoryJson)
{
    std::vector<GameRulesData> toReturn;
    toReturn.reserve(gameRulesCurrentCategoryJson->Values.Num());
    for (const auto& gameRulesCurrentCategory : gameRulesCurrentCategoryJson->Values)
    {
        GameRulesData gameRulesDataToPush;
        const FString& gameRulesCurrentCategoryKey = gameRulesCurrentCategory.Key;
        gameRulesDataToPush.header = gameRulesCurrentCategoryKey;

        const TArray<TSharedPtr<FJsonValue>>* gameRulesCurrentCategoryArrayPointer = nullptr;
        if (gameRulesCurrentCategoryJson->TryGetArrayField(gameRulesCurrentCategoryKey, gameRulesCurrentCategoryArrayPointer))
        {
            for (const TSharedPtr<FJsonValue>& value : *gameRulesCurrentCategoryArrayPointer)
            {
                FString toPush;
                if (value->TryGetString(toPush))
                {
                    gameRulesDataToPush.contents.Append(toPush);
                    gameRulesDataToPush.contents.Append(" ");
                }
            }

            toReturn.push_back(gameRulesDataToPush);
        }
    }

    return toReturn;
}