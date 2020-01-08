#pragma once
#include <functional>

class NetworkMessagesHandler
{
public:
    void Init();

    void ProcessMessage(const TSharedPtr<FJsonObject>& jsonObject);

private:
    //void OnNetAllAreReadyMessage(const TSharedPtr<FJsonObject>& jsonObject);

private:
    TMap<FString, std::function<void(const TSharedPtr<FJsonObject>&)>> m_NetEventsCallbacks;
};