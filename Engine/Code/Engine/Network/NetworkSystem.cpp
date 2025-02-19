#include "Engine/Network/NetworkSystem.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Core/DevConsole.hpp"

NetWorkSystem* g_theNetwork = nullptr;

NetWorkSystem::NetWorkSystem(NetworkConfig config)
	:m_config(config)
{
	m_mode = NetMode::NONE;
	m_clientSocket = INVALID_SOCKET;
}

NetWorkSystem::~NetWorkSystem()
{

}

void NetWorkSystem::Startup()
{
#if defined(ENGINE_DISABLE_NETWORK)
	return;
#endif
	if (!IsEnable()) return;

	InitializeWinsock();

	m_sendBuffer = new char[m_config.m_sendBufferSize];
	m_recvBuffer = new char[m_config.m_recvBufferSize];

	if (ToLower(m_config.m_modeString) == "client") 
	{
		m_mode = NetMode::CLIENT;
		g_theDevConsole->AddLine(Rgba8(255, 255, 255), "Client");
	}
	else if (ToLower(m_config.m_modeString) == "server") 
	{
		m_mode = NetMode::SERVER;
		g_theDevConsole->AddLine(Rgba8(255, 255, 255), "Server");
	}

	if (m_mode == NetMode::CLIENT)
	{
		m_mode = NetMode::CLIENT;
		CreateClientSocket();
	}
	else if (m_mode == NetMode::SERVER)
	{
		m_mode = NetMode::SERVER;
		CreateAndBindServerSocket();;
	}

	SubscribeEventCallbackFunction("BurstTest", NetWorkSystem::BurstTest);
	SubscribeEventCallbackFunction("RemoteCommand", NetWorkSystem::RemoteCommand);
}

void NetWorkSystem::Shutdown()
{
#if defined(ENGINE_DISABLE_NETWORK)
	return;
#endif
	if (!IsEnable()) return;

	if (m_clientSocket != INVALID_SOCKET)
	{
		closesocket(m_clientSocket);
		m_clientSocket = INVALID_SOCKET;
	}
	if (m_listenSocket != INVALID_SOCKET)
	{
		closesocket(m_listenSocket);
		m_listenSocket = INVALID_SOCKET;
	}
	WSACleanup();

	delete[] m_sendBuffer;
	delete[] m_recvBuffer;
	m_sendBuffer = nullptr;
	m_recvBuffer = nullptr;
}

void NetWorkSystem::BeginFrame()
{
#if defined(ENGINE_DISABLE_NETWORK)
	return;
#endif
	if (!IsEnable()) return;
                                                                                                                            
	if (m_mode == NetMode::CLIENT) {
		// Check if our connection attempt has completed.
		sockaddr_in addr;
		addr.sin_family = AF_INET;
		addr.sin_addr.S_un.S_addr = htonl(m_hostAddress);
		addr.sin_port = htons(m_hostPort);
		int result = connect(m_clientSocket, (sockaddr*)(&addr), (int)sizeof(addr));

		fd_set writeSockets;
		fd_set exceptSockets;
		FD_ZERO(&writeSockets);
		FD_ZERO(&exceptSockets);
		FD_SET(m_clientSocket, &writeSockets);
		FD_SET(m_clientSocket, &exceptSockets);
		timeval waitTime = { };
		result = select(0, NULL, &writeSockets, &exceptSockets, &waitTime);

		// We are connected if the following is true.
		if (result >= 0) 
		{
			if (FD_ISSET(m_clientSocket, &writeSockets)) 
			{
				// Send and receive if we are connected.
				m_clientState = ClientState::Connected;
				if (!ProcessMessage()) {
					goto PrintState;
				}
			}
			else if (FD_ISSET(m_clientSocket, &exceptSockets)) 
			{
				// Attempt to connect if we haven't already.
				addr.sin_family = AF_INET;
				addr.sin_addr.S_un.S_addr = htonl(m_hostAddress);
				addr.sin_port = htons(m_hostPort);
				result = connect(m_clientSocket, (sockaddr*)(&addr), (int)sizeof(addr));
				if (result <= 0) 
				{
					int error = WSAGetLastError();
					if (error == WSAEWOULDBLOCK || error == WSAEALREADY) 
					{
						goto PrintState;
					}
				}

				// Check if our connection attempt failed.
				FD_ZERO(&exceptSockets);
				FD_SET(m_clientSocket, &exceptSockets);
				waitTime = { };
				result = select(0, NULL, &writeSockets, &exceptSockets, &waitTime);

				// The connection failed if the following is true, in which case we need to connect again.
				if (result > 0 && FD_ISSET(m_clientSocket, &exceptSockets) && FD_ISSET(m_clientSocket, &writeSockets)) 
				{
					m_clientState = ClientState::Connected;
				}
			}
			else 
			{
				if (HandleErrors()) 
				{
					goto PrintState;
				}
			}
		}
		else if (result < 0) 
		{
			if (HandleErrors()) 
			{
				goto PrintState;
			}
		}

	PrintState:
		if (m_lastFrameClientState == ClientState::Disconnected && m_clientState == ClientState::Connected) 
		{
			g_theDevConsole->AddLine(Rgba8(255, 255, 255), Stringf("Connected to Server %s! Socket: %lld", 
				m_config.m_hostAddressString.c_str(), m_clientSocket));
		}
		else if (m_lastFrameClientState == ClientState::Connected && m_clientState == ClientState::Disconnected) 
		{
			g_theDevConsole->AddLine(Rgba8(255, 255, 255), Stringf("Disconnected from Server %s! Socket: %lld", 
				m_config.m_hostAddressString.c_str(), m_clientSocket));
		}
		m_lastFrameClientState = m_clientState;
	}
	else if (m_mode == NetMode::SERVER) 
	{
		// If we do not have a connection, check for connections to accept.
		if (m_clientSocket == INVALID_SOCKET) 
		{
			m_clientSocket = accept(m_listenSocket, NULL, NULL);
			if (m_clientSocket != INVALID_SOCKET) 
			{
				// If a connection is accepted set blocking mode.
				unsigned long blockingMode = 1;
				ioctlsocket(m_clientSocket, FIONBIO, &blockingMode);
				g_theDevConsole->AddLine(Rgba8(255, 255, 255), Stringf("Connected to Client! Socket: %lld", m_clientSocket));
			}
		}
		else {
		
			if (!ProcessMessage()) 
			{
				return;
			}
		}

	}

}

void NetWorkSystem::EndFrame()
{
#if defined(ENGINE_DISABLE_NETWORK)
	return;
#endif
	if (!IsEnable()) return;
}

bool NetWorkSystem::IsEnable() const
{
	return m_config.m_isEnable;
}

bool NetWorkSystem::IsClient() const
{
	return m_mode == NetMode::CLIENT;
}

bool NetWorkSystem::IsServer() const
{
	return m_mode == NetMode::SERVER;
}

bool NetWorkSystem::IsConnected() const
{
	return m_clientState == ClientState::Connected;
}

void NetWorkSystem::Send(std::string data)
{
	m_sendQueue.push_back(data);
}

bool NetWorkSystem::RemoteCommand(EventArgs& args)
{
	std::string line = args.GetValue("command", "");
	Strings totalCommands = SplitStringWithQuotes(line, ' ');
	for (auto & cmd : totalCommands)
	{
		TrimString(cmd,'\"');
		ToLower(cmd);
		g_theNetwork->Send(Stringf(cmd.c_str()));
	}

	return true;
}

bool NetWorkSystem::BurstTest(EventArgs& args)
{
	for (size_t i = 0; i < 20; i++)
	{
		args.SetValue("command", Stringf("\"echo message=%i\"", i));
		g_theNetwork->RemoteCommand(args);
	}
	return true;
}

void NetWorkSystem::ExecuteRecvMessage(std::string const& message)
{
	g_theDevConsole->Execute(message, true);
}

void NetWorkSystem::InitializeWinsock()
{
	WSADATA wsaData;
	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result != 0)
	{
		ERROR_AND_DIE("Failed to initialize Winsock");
	}
}

void NetWorkSystem::CreateAndBindServerSocket()
{
	// Create listen socket.
	m_listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_listenSocket == INVALID_SOCKET)
	{
		WSACleanup();
		ERROR_AND_DIE(Stringf("Error at socket(): %ld\n", WSAGetLastError()));
	}

	// Set blocking mode.
	unsigned long blockingMode = 1;
	int result = ioctlsocket(m_listenSocket, FIONBIO, &blockingMode);

	// Get host port from string.
	Strings IPAndPort;
	SplitStringOnDelimiter(IPAndPort, m_config.m_hostAddressString, ":");
	m_hostAddress = INADDR_ANY;
	m_hostPort = (unsigned short)(atoi(IPAndPort[1].c_str()));

	// Bind the listen socket to a port.
	sockaddr_in addr = { };
	addr.sin_family = AF_INET;
	addr.sin_addr.S_un.S_addr = htonl(m_hostAddress);
	addr.sin_port = htons(m_hostPort);
	result = bind(m_listenSocket, (sockaddr*)&addr, (int)sizeof(addr));
	if (result == SOCKET_ERROR) 
	{
		WSACleanup();
		ERROR_AND_DIE(Stringf("bind failed with error: %d\n", WSAGetLastError()));
	}

	// Listen for connections to accept.
	result = listen(m_listenSocket, SOMAXCONN);
}

void NetWorkSystem::CreateClientSocket()
{
	m_clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_clientSocket == INVALID_SOCKET) 
	{
		WSACleanup();
		ERROR_AND_DIE(Stringf("Error at socket(): %ld\n", WSAGetLastError()));
	}

	// Set blocking mode
	unsigned long blockingMode = 1;
	int result = ioctlsocket(m_clientSocket, FIONBIO, &blockingMode);

	// Get host address from string.
	Strings IPAndPort;
	SplitStringOnDelimiter(IPAndPort, m_config.m_hostAddressString, ":");
	IN_ADDR addr = { };
	result = inet_pton(AF_INET, IPAndPort[0].c_str(), &addr);

	m_hostAddress = ntohl(addr.S_un.S_addr);

	// Get host port from string
	m_hostPort = (unsigned short)(atoi(IPAndPort[1].c_str()));
}
bool NetWorkSystem::HandleErrors()
{
	int error = WSAGetLastError();
	if (error == WSAECONNABORTED || error == WSAECONNRESET || error == 0) 
	{
		if (m_mode == NetMode::SERVER) 
		{
			m_clientSocket = INVALID_SOCKET;
		}
		else if (m_mode == NetMode::CLIENT)
		{
			m_clientState = ClientState::Disconnected;
			CreateClientSocket();
		}
	}
	else if (error == WSAEWOULDBLOCK || error == WSAEALREADY) 
	{
		return true;
	}
	else 
	{
		g_theDevConsole->AddLine(DevConsole::ERROR, Stringf("Error code: %d", error));
	}
	return false;
}

bool NetWorkSystem::ProcessMessage()
{
	while (!m_sendQueue.empty())
	{
		//m_sendBuffer = m_sendQueue[0].c_str();
		const std::string& front = m_sendQueue.front();
		int msgLength = (int)front.length();
		int numBytesSent = 0;
		while (numBytesSent < msgLength)
		{
			int numByteToSent = IntMin(msgLength - numBytesSent, m_config.m_recvBufferSize - 1);

			memset(m_sendBuffer, 0, m_config.m_sendBufferSize);

			strncpy_s(m_sendBuffer, m_config.m_sendBufferSize, front.c_str() + numBytesSent, numByteToSent);

			if (numBytesSent + numByteToSent >= msgLength)
			{
				m_sendBuffer[numByteToSent] = '\0';
				numByteToSent++;
			}

			int resultSent = send(m_clientSocket, m_sendBuffer, numByteToSent, 0);

			if (resultSent == 0)
			{
				shutdown(m_clientSocket, SD_BOTH);
				closesocket(m_clientSocket);
				m_clientSocket = INVALID_SOCKET;
				return false;
			}
			if (resultSent == SOCKET_ERROR)
			{
				if (HandleErrors())
				{
					return false;
				}
			}

			numBytesSent += resultSent;
		}
		m_sendQueue.pop_front();
	}

	int resultRcv = recv(m_clientSocket, m_recvBuffer, m_config.m_recvBufferSize, 0);

	if (resultRcv > 0)
	{
		m_recvQueue.append(m_recvBuffer, resultRcv);
		memset(m_recvBuffer, 0, m_config.m_recvBufferSize);

		size_t pos;
		while ((pos = m_recvQueue.find('\0')) != std::string::npos)
		{
			std::string completeMsg = m_recvQueue.substr(0, pos);

			ExecuteRecvMessage(completeMsg);

			m_recvQueue.erase(0, pos + 1);
		}
	}

	if (resultRcv == 0)
	{
		shutdown(m_clientSocket, SD_BOTH);
		closesocket(m_clientSocket);
		m_clientSocket = INVALID_SOCKET;
		return false;
	}
	if (resultRcv == SOCKET_ERROR)
	{
		if (HandleErrors())
		{
			return false;
		}
	}


	return true;
}