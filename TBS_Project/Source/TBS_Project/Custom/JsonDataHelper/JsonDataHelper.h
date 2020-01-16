#pragma once
#include "Custom/Singleton/Singleton.h"
#include "Custom/JsonDataHelper/GameRulesDataHelper.h"


class JsonDataHelper : public Singleton<JsonDataHelper>
{
    using Super = Singleton;

public:
    const GameRulesDataHelper& GetGameRulesDataHelper() const { return m_GameRulesDataHelper; }

    void Init() override;
    void Shutdown() override;

private:
    GameRulesDataHelper m_GameRulesDataHelper; //#TODO reuse after creating TBS game rules
};
