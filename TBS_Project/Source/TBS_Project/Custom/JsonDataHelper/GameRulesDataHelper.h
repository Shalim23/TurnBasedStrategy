#pragma once
#include <vector>

struct GameRulesData
{
    FString header;
    FString contents;
};

struct GameRulesCategoryData
{
    FString header;
    std::vector<GameRulesData> gameRulesData;
};

class GameRulesDataHelper
{
public:
    void InitGameRulesData();

    const std::vector<GameRulesCategoryData>& GetData() const { return m_Data; }

private:
    std::vector<GameRulesData> ProcessGameRulesCategory(const TSharedPtr<FJsonObject>& gameRulesCurrentCategoryJson);

private:
    std::vector<GameRulesCategoryData> m_Data;

};
