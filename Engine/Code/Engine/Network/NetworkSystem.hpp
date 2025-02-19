#pragma once

#define WIN32_LEAN_AND_MEAN	
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")

#include "Engine/Core/StringUtils.hpp"
#include "Engine/Core/EventSystem.hpp"
#include "Engine/Core/DevConsole.hpp"
#include <deque>

enum class NetMode
{
	NONE = 0,
	CLIENT,
	SERVER
};

enum class ClientState 
{
	Disconnected,
	Connected,
	Disabled,
};

struct NetworkConfig
{
	bool m_isEnable = true;
	std::string m_modeString;
	std::string m_hostAddressString;
	int m_sendBufferSize = 2048;
	int m_recvBufferSize = 2048;

};

class NetWorkSystem
{
public:
	NetWorkSystem(NetworkConfig config);
	virtual ~NetWorkSystem();

	void						Startup();
	void						Shutdown();
	virtual void				BeginFrame();
	virtual void				EndFrame();

	bool			IsEnable() const;
	bool			IsClient() const;
	bool			IsServer() const;
	bool			IsConnected() const;

	void Send(std::string data);

	static bool RemoteCommand(EventArgs& args);
	static bool BurstTest(EventArgs& args);


private:
	NetworkConfig m_config;
	NetMode m_mode;
	ClientState  m_clientState = ClientState::Disconnected;
	ClientState	m_lastFrameClientState = ClientState::Disconnected;
	uintptr_t m_clientSocket;
	uintptr_t m_listenSocket;
	unsigned long m_hostAddress = 0;
	unsigned short m_hostPort = 0;
	char* m_sendBuffer;
	char* m_recvBuffer;
	std::deque<std::string> m_sendQueue;
	std::string m_recvQueue;

protected:
	void ExecuteRecvMessage(std::string const& message);
	void InitializeWinsock();
	void CreateAndBindServerSocket();
	bool HandleErrors();
	void CreateClientSocket();
	bool ProcessMessage();
};

extern NetWorkSystem* g_theNetwork;