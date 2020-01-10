using System.Collections.Generic;
using System;

using static TBS_GameServer.Source.Events.Delegates;

namespace TBS_GameServer.Source.Events
{
    class EventsManagerInstance
    {
        public bool TryGetDelegate<T>(DelegateType type, out T requiredDelegate) where T : Delegate
        {
            if (m_Delegates.ContainsKey(type))
            {
                requiredDelegate = (T)m_Delegates[type];
                return true;
            }

            requiredDelegate = null;
            return false;
        }

        public void Subscribe<T>(DelegateType type, T functor) where T : Delegate
        {
            if(m_Delegates.ContainsKey(type))
            {
                var combinedDelegates = Delegate.Combine(m_Delegates[type], functor);
                m_Delegates[type] = combinedDelegates;
            }
            else
            {
                Console.Write($"{type.ToString()} was not found in Subscribe");
            }
        }

        public void Init()
        {
            m_Delegates = new Dictionary<DelegateType, Delegate>();
            NetworkMessageDelegate networkMessageDelegate = null;
            ConnectionErrorDelegate connectionErrorDelegate = null;

            m_Delegates.Add(DelegateType.ConnectionError, connectionErrorDelegate);
            m_Delegates.Add(DelegateType.NetworkMessage, networkMessageDelegate);
        }

        Dictionary<DelegateType, Delegate> m_Delegates;
    }
}
