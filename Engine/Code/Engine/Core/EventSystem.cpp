#include "Engine/Core/EventSystem.hpp"
#include "Engine/Core/DevConsole.hpp"

EventSystem* g_theEventSystem = nullptr;

EventSystem::EventSystem(EventSystemConfig const& config)
	:m_config(config)
{

}

EventSystem::~EventSystem()
{
}

void EventSystem::Startup()
{
}

void EventSystem::BeginFrame()
{
}

void EventSystem::EndFrame()
{
}

void EventSystem::Shutdown()
{
}

void EventSystem::SubscribeEventCallbackFunction(std::string const& eventName, EventCallbackFunction functionPtr)
{
	m_subscriptionListByEventName[ToLower(eventName)].push_back(EventSubscription(functionPtr));
}

void EventSystem::UnsubscribeEventCallbackFunction(std::string const& eventName, EventCallbackFunction functionPtr)
{
	std::map<std::string, SubscriptionList>::iterator found = m_subscriptionListByEventName.find(ToLower(eventName));
	if (found != m_subscriptionListByEventName.end())
	{
		SubscriptionList& subscribersForThisEvent = found->second;
		for (int i = 0; i < (int)subscribersForThisEvent.size(); i++)
		{
			EventSubscription& subscriber = subscribersForThisEvent[i];
			if (subscriber.m_functionPtr == functionPtr)
			{
				subscribersForThisEvent.erase(subscribersForThisEvent.begin() + i);
				--i;
			}
		}
	}
}

void EventSystem::FireEvent(std::string const& eventName, EventArgs& arg)
{
	//m_eventSystemMutex.lock();
	std::map<std::string, SubscriptionList>::iterator found = m_subscriptionListByEventName.find(ToLower(eventName));
	if (found != m_subscriptionListByEventName.end())
	{
		SubscriptionList& subscribersForThisEvent = found->second;
		for (int i = 0; i < (int)subscribersForThisEvent.size(); i++)
		{
			EventSubscription& subscriber = subscribersForThisEvent[i];
			bool wasConsumed = subscriber.m_functionPtr(arg);
			if (wasConsumed)
			{
				break;
			}
		}
	}
	else
	{
		if (g_theDevConsole)
		{
			g_theDevConsole->AddLine(DevConsole::ERROR, "ERROR: Unknown command: " + ToLower(eventName));
		}
	}
	//m_eventSystemMutex.unlock();
}

void EventSystem::FireEvent(std::string const& eventName)
{
	EventArgs arg;
	FireEvent(ToLower(eventName), arg);
}

Strings EventSystem::GetAllRegisteredEvent()
{
	Strings allResgisteredEvent;
	for (const std::pair<std::string, SubscriptionList>& pair : m_subscriptionListByEventName)
	{
		const std::string& eventName = pair.first;
		allResgisteredEvent.push_back(eventName);
	}
	return allResgisteredEvent;
}

void SubscribeEventCallbackFunction(std::string const& eventName, EventCallbackFunction functionPtr)
{
	g_theEventSystem->SubscribeEventCallbackFunction(ToLower(eventName), functionPtr);
}

void UnsubscribeEventCallbackFunction(std::string const& eventName, EventCallbackFunction functionPtr)
{
	g_theEventSystem->UnsubscribeEventCallbackFunction(ToLower(eventName), functionPtr);
}

void FireEvent(std::string const& eventName, EventArgs& arg)
{
	g_theEventSystem->FireEvent(ToLower(eventName), arg);
}

void FireEvent(std::string const& eventName)
{
	g_theEventSystem->FireEvent(ToLower(eventName));
}

EventSubscription::EventSubscription(EventCallbackFunction functionPtr)
	:m_functionPtr(functionPtr)
{
}
