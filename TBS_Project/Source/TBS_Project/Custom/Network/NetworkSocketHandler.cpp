#include "NetworkSocketHandler.h"
#include "Networking.h"
#include "Serialization/JsonSerializer.h"


void NetworkSocketHandler::SetMessageFromServerCallback(std::function<void(const TSharedPtr<FJsonObject>&)> callback)
{
    m_MessageFromServerCallback = callback;
}

void NetworkSocketHandler::SetSendErrorCallback(std::function<void()> callback)
{
    m_SendErrorCallback = callback;
}

void NetworkSocketHandler::Shutdown()
{
    if (m_Socket)
    {
        m_Socket->Close();
        m_Socket = nullptr;
    }
}

void NetworkSocketHandler::Receive()
{
    m_Socket->Recv(m_Buffer, m_NetMessageSize, m_BytesRead);
    if (m_BytesRead > 0)
    {
        FString jsonString(UTF8_TO_TCHAR(m_Buffer));
        TSharedPtr<FJsonObject> jsonObject = MakeShareable(new FJsonObject());
        TSharedRef<TJsonReader<>> jsonReader = TJsonReaderFactory<>::Create(jsonString);

        if (FJsonSerializer::Deserialize(jsonReader, jsonObject) && jsonObject.IsValid())
        {
            const TSharedPtr<FJsonObject>* messageObject = nullptr;
            if (jsonObject->TryGetObjectField(TBSGameMessageJsonKey, messageObject))
            {
                ProcessReceivedData(*messageObject);
            }
        }

        m_BytesRead = 0;
        memset(m_Buffer, 0, sizeof(m_Buffer));
    }
}

bool NetworkSocketHandler::TryConnect()
{
    m_Socket = FTcpSocketBuilder("default").AsNonBlocking();

    FIPv4Address ip(127, 0, 0, 1);

    TSharedRef<FInternetAddr> Addr =
        ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();

    Addr->SetIp(ip.Value);
    Addr->SetPort(56000);

    m_Socket->Connect(*Addr);
    if (m_Socket->GetConnectionState() == SCS_Connected)
    {
        return true;
    }

    Shutdown();

    return false;
}


void NetworkSocketHandler::Send(const TSharedPtr<FJsonObject>& jsonObject)
{
    TSharedPtr<FJsonObject> rootObject = MakeShareable(new FJsonObject);
    rootObject->SetObjectField(TBSGameMessageJsonKey, jsonObject);

    FString jsonString;
    TSharedRef<TJsonWriter<>> writer = TJsonWriterFactory<>::Create(&jsonString);
    FJsonSerializer::Serialize(rootObject.ToSharedRef(), writer);
    
    int32 sent = 0;
    m_Socket->Send(TArray<uint8>(jsonString.GetCharArray()).GetData(), jsonString.Len(), sent);

    int error = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->GetLastErrorCode();
    if (error == SE_ECONNRESET && m_SendErrorCallback)
    {
        m_SendErrorCallback();
    }
}

void NetworkSocketHandler::ProcessReceivedData(const TSharedPtr<FJsonObject>& jsonObject)
{
    if (m_MessageFromServerCallback)
    {
        m_MessageFromServerCallback(jsonObject);
    }
}