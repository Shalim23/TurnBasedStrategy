#include "JsonDataHelper.h"

void JsonDataHelper::Init()
{
    m_GameRulesDataHelper.InitGameRulesData();
}

void JsonDataHelper::Shutdown()
{
    m_GameRulesDataHelper.Clear();
}
