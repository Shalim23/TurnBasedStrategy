using TBS_GameServer.Network.PlayerConnection;
using System.Runtime.CompilerServices;
using TBS_GameServer.Events;
using static TBS_GameServer.Events.Delegates;


namespace TBS_GameServer.Network
{
    class NetworkManager
    {
        static NetworkManager instance = null;

        [MethodImpl(MethodImplOptions.Synchronized)]
        public static NetworkManager GetInstance()
        {
            if (instance == null)
            {
                instance = new NetworkManager();
            }
               
            return instance;
        }

        void SubscribeOnEvents()
        {
            EventsManager eventsManager = EventsManager.GetInstance();

            eventsManager.Subscribe<ConnectionErrorDelegate>(DelegateType.ConnectionError, OnConnectionError);
        }

        public void Init()
        {
            m_SocketsHandler = new SocketsHandler();

            m_MessageHandler = new MessageHandler();
            m_MessageHandler.Init();
            SubscribeOnEvents();
        }

        public void StartMessageProcessing()
        {
            while(m_IsActive)
            {
                m_SocketsHandler.ProcessReceive();
            }
        }

        public void ProcessPlayerConnection()
        {
            PlayerConnectionHandler playerConnector = new PlayerConnectionHandler();
            m_SocketsHandler.Activate(playerConnector.GetReadyPlayers());

            m_IsActive = true;
        }

        public void CleanUp()
        {
            m_MessageHandler = null;
            m_SocketsHandler = null;
        }

        void OnConnectionError()
        {
            m_SocketsHandler.SendConnectionError();
            m_IsActive = false;
        }

        SocketsHandler m_SocketsHandler = null;
        MessageHandler m_MessageHandler = null;
        bool m_IsActive = false;
    }
}
