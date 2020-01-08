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
	void unsubscribe();

private:
    std::vector<EventDelegateHandle> m_delegateHandles;
};
