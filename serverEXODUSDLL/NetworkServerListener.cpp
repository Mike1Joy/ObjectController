// Author/editor/coder: Simo

#include <winsock2.h>
#include <Windows.h>
#include <WS2tcpip.h>
#include <IPHlpApi.h>
#include <stdio.h>
#include <iostream>
#include "exodus.h"

#pragma comment(lib, "Ws2_32.lib")

// #pragma comment(lib, "Mswsock.lib")

#include "NetworkServerConnection.h"
#include "NetworkServerClient.h"
#include "NetworkServerHandler.h"

#include "NetworkServerListener.h"

CNetworkServerListener::CNetworkServerListener(char * pszPort)
	: CAbstractThread()
{
	m_bListening = false;
	m_bWantMoreConnections = false;
	m_pListenSocket = NULL;
    m_pszPort = pszPort;
}


CNetworkServerListener::~CNetworkServerListener(void)
{
	if(NULL != m_pListenSocket)
	{
		SOCKET ListenSocket = *((SOCKET *)m_pListenSocket);
		m_bWantMoreConnections = false;
		closesocket(ListenSocket);
	}
    if (nullptr != m_pszPort)
    {
        delete m_pszPort;
    }
}

void 
CNetworkServerListener::StartListening()
{
	m_bWantMoreConnections = true;
	if(m_bListening)
	{
		return;
	}
	Start();
}
void 
CNetworkServerListener::StopListening()
{
	m_bWantMoreConnections = false;
}

int 
CNetworkServerListener::Run()
{
	Listen();
	return 0;
}

void 
CNetworkServerListener::Listen()
{
	m_bListening = true;
	WSADATA wsaData;
	int iResult;

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
	if(iResult != 0)
	{
        formated_output("WSAStartup failed: %d\n", iResult);
		m_bListening = false;
		return;
	}
	
	// Create socket
	//char port[50] = "8000\0";
	struct addrinfo *result = NULL, *ptr = NULL, hints;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	// Resolve the local address and port to be used by the server
	iResult = getaddrinfo(NULL, m_pszPort, &hints, &result);
	if(iResult != 0){
        formated_output("getaddrinfo failed: %d\n", iResult);
		WSACleanup();
		m_bListening = false;
		return;
	}

	SOCKET ListenSocket = INVALID_SOCKET;
	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	m_pListenSocket = (void *)&ListenSocket;

	if(ListenSocket == INVALID_SOCKET){
        formated_output("Error at socket(): %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		m_bListening = false;
		return;
	}

	//cout << "Binding, ";
	// Bind socket
	iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
	if(iResult == SOCKET_ERROR){
        formated_output("bind failed with error: %d\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(ListenSocket);
		WSACleanup();
		m_bListening = false;
		return;
	}
	freeaddrinfo(result); // once bind is called, getaddrinfo is no longer needed

	//cout << "Listening, ";

	// Listen on a socket
	if(listen(ListenSocket, SOMAXCONN) == SOCKET_ERROR){
        formated_output("Listen failed with error: %ld\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		m_bListening = false;
		return;
	}

	// Accept a connection
	SOCKET ClientSocket;
	ClientSocket = INVALID_SOCKET;

	//cout << "Waiting for connection...\n\r";
	// Accept a client socket
	while(m_bWantMoreConnections)
	{
		ClientSocket = accept(ListenSocket, NULL, NULL);
		if(ClientSocket == INVALID_SOCKET)
		{
            formated_output("accept failed: %d\n", WSAGetLastError());
			closesocket(ListenSocket);
			WSACleanup();
			m_bListening = false;
			return;
		}
        StaticWriteLineToWindow("New Client");
		CNetworkServerConnection * pConnection = new CNetworkServerConnection((void *)&ClientSocket);
		CNetworkServerClient * pClient = new CNetworkServerClient(CNetworkServerHandler::GetNextClientId(), pConnection);
		CNetworkServerHandler::AddClient(pClient);
	}
	//cout << "Connection accepted!\n";

	// No longer need server socket
	closesocket(ListenSocket);
}