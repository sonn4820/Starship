#pragma once
#include "Engine/Window/Window.hpp"
#include "Engine/Core/NamedStrings.hpp"
#include "Engine/Core/StringUtils.hpp"
#include <string>
#include <vector>
#include <mutex>


typedef NamedStrings EventArgs;
typedef bool (*EventCallbackFunction)(EventArgs&);
struct EventSubscription
{
	EventSubscription(EventCallbackFunction functionPtr);
	EventCallbackFunction m_functionPtr = nullptr;
};
typedef std::vector<EventSubscription> SubscriptionList;

struct EventSystemConfig
{
	Window* m_window = nullptr;
};


class EventSystem
{
public:
	EventSystem(EventSystemConfig const& config);
	~EventSystem();

	void Startup();
	void BeginFrame();
	void EndFrame();
	void Shutdown();

	void SubscribeEventCallbackFunction(std::string const& eventName, EventCallbackFunction functionPtr);
	void UnsubscribeEventCallbackFunction(std::string const& eventName, EventCallbackFunction functionPtr);
	void FireEvent(std::string const& eventName, EventArgs& arg);
	void FireEvent(std::string const& eventName);
	Strings GetAllRegisteredEvent();

protected:
	EventSystemConfig m_config;
	std::map<std::string, SubscriptionList> m_subscriptionListByEventName;

};

extern EventSystem* g_theEventSystem;
void SubscribeEventCallbackFunction(std::string const& eventName, EventCallbackFunction functionPtr);
void UnsubscribeEventCallbackFunction(std::string const& eventName, EventCallbackFunction functionPtr);
void FireEvent(std::string const& eventName, EventArgs& arg);
void FireEvent(std::string const& eventName);