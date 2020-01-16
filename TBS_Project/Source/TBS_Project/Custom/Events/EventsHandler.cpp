#include "EventsHandler.h"
#include "Custom/Events/EventDispatcher.h"

EventsHandler::EventsHandler()
{}

EventsHandler::~EventsHandler()
{
    if (!m_delegateHandles.empty())
    {
        EventDispatcher::GetInstance().unsubscribe(m_delegateHandles);
        m_delegateHandles.clear();
    }
}

void EventsHandler::subscribe(const std::vector<EventSubscribeData>& callbacks)
{
    m_delegateHandles = EventDispatcher::GetInstance().subscribe(callbacks);
}
