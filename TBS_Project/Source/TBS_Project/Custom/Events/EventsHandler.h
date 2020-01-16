#pragma once
#include <vector>

struct EventSubscribeData;
struct EventDelegateHandle;

class EventsHandler
{
public:
	EventsHandler();
	~EventsHandler();

	void subscribe(const std::vector<EventSubscribeData>& callbacks);

private:
    std::vector<EventDelegateHandle> m_delegateHandles;
};
