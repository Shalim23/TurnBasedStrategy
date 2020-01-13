#pragma once
#include <functional>
#include "NetworkData.h"

class FBufferArchive;
class FSocket;

class NetworkSocketHandler
{

public:
    void SetMessageFromServerCallback(std::function<void(const TSharedPtr<FJsonObject>&)> callback);
    void SetSendErrorCallback(std::function<void()> callback);
    void Reset();

    bool TryConnect();

    void Send(const TSharedPtr<FJsonObject>& jsonObject);
    void Receive();

private:
    void ProcessReceivedData(const TSharedPtr<FJsonObject>& jsonObject);

private:
    FSocket* m_Socket = nullptr;

    const static size_t m_NetMessageSize = 1024;
    uint8 m_Buffer[m_NetMessageSize] = {};
    int32 m_BytesRead = 0;

    std::function<void(const TSharedPtr<FJsonObject>&)> m_MessageFromServerCallback;
    std::function<void()> m_SendErrorCallback;
};
