#pragma once
#include <functional>

class NetworkMessagesHandler
{
public:
    void Init();

    void ProcessMessage(const TSharedPtr<FJsonObject>& jsonObject);

private:
    void OnNetAllAreReadyMessage(const TSharedPtr<FJsonObject>& jsonObject);
    void OnNetNotReadyMessage(const TSharedPtr<FJsonObject>& jsonObject);
    void OnNetWaitingForReadinessMessage(const TSharedPtr<FJsonObject>& jsonObject);
    void OnNetWaitingForPlayersMessage(const TSharedPtr<FJsonObject>& jsonObject);

private:
    TMap<FString, std::function<void(const TSharedPtr<FJsonObject>&)>> m_NetEventsCallbacks;
};