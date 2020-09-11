// Author/editor/coder: Simo
#include <winsock2.h>
#include <Windows.h>
#include <WS2tcpip.h>
#include <IPHlpApi.h>
#include <stdio.h>
#include <iostream>

#pragma comment(lib, "Ws2_32.lib")

//#pragma comment(lib, "Mswsock.lib")

#include "Endianness.h"
#include "NetworkClientConnection.h"
CNetworkClientConnection::CNetworkClientConnection(CStr * pAddr, CStr * pPort)
{
	m_bConnected = false;
	m_bRunning = false;
	m_pAddress = new CStr(pAddr);
	m_pPort = new CStr(pPort);
	m_pLeftOvers = nullptr;
	m_iLeftOversLength = 0;
	m_iLeftOversOffset = 0;
	m_pSender = nullptr;
	m_pSocket = nullptr;
	m_bConnectionTried = false;
}


CNetworkClientConnection::~CNetworkClientConnection(void)
{
	if(nullptr != m_pLeftOvers)
	{
		delete m_pLeftOvers;
	}
    CNetworkMessage * pMsg = nullptr;
	while(nullptr != (pMsg = m_receiveMessages.Take()))
	{
		delete pMsg;
	}
	while(nullptr != (pMsg = m_sendMessages.Take()))
	{
		delete pMsg;
	}
	if(nullptr != m_pSender)
	{
		delete m_pSender;
	}
	if(nullptr != m_pAddress)
	{
		delete m_pAddress;
	}
	if(nullptr != m_pPort)
	{
		delete m_pPort;
	}
}

bool 
CNetworkClientConnection::Connect()
{
	if(nullptr == m_pAddress || m_pAddress->Length() < 1)
	{
		m_bConnectionTried = true; // Invalid address, so we tried..
		return false;
	}
	if(nullptr == m_pPort || m_pPort->Length() < 1)
	{
		m_bConnectionTried = true; // Invalid port, so we tried..
		return false;
	}
	Start();
	return true;
}
bool 
CNetworkClientConnection::Disconnect()
{
	if(!m_bConnected)
	{
		return false;
	}
	// cleanup
	if(nullptr != m_pSocket)
	{
		SOCKET ConnectSocket = *((SOCKET *)m_pSocket);
		closesocket(ConnectSocket);
	}
	WSACleanup();
	return true;
}
int 
CNetworkClientConnection::Run()
{
	if(nullptr == m_pAddress || m_pAddress->Length() < 1)
	{
		m_bConnectionTried = true; // Invalid address, so we tried..
		return -1;
	}
	if(nullptr == m_pPort || m_pPort->Length() < 1)
	{
		m_bConnectionTried = true; // Invalid port, so we tried..
		return -1;
	}
	m_bRunning = true;
	WSADATA wsaData;
	int iResult;
	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
	if(iResult != 0){
		printf("WSAStartup failed: %d\n", iResult);
		m_bRunning = false;
		m_bConnectionTried = true; // Socket error, we tried..
		return -1;
	}

	struct addrinfo *result = nullptr, 
		*ptr = nullptr,
		hints;
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET; // AF_UNSPEC
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	//char defaultPort[50] = "8000\0";
	//char defaultAddress[50] = "127.0.0.1\0";

	// Resolve the server address and port
	iResult = getaddrinfo(m_pAddress->GetArray(), m_pPort->GetArray(), &hints, &result);
	if(iResult != 0){
		printf("getaddrinfo failed: %d\n", iResult);
		WSACleanup();
		m_bRunning = false;
		m_bConnectionTried = true; // Address error, we tried..
		return -1;
	}

	SOCKET ConnectSocket = INVALID_SOCKET;

	// Attempt to connect to the first address returned by
	// the call to getaddrinfo
	ptr=result;

	// Create a SOCKET for connecting to server
	ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

	if(ConnectSocket == INVALID_SOCKET){
		printf("Error at socket(): %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		m_bRunning = false;
		m_bConnectionTried = true; // Socket error, we tried..
		return -1;
	}

	// Connect to server.
	iResult = connect( ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
	if(iResult == SOCKET_ERROR){
		closesocket(ConnectSocket);
		ConnectSocket = INVALID_SOCKET;
	}

	// Should really try the next address returned by getaddrinfo
	// if the connect call failed
	// But for this simple example we just free the resources
	// returned by getaddrinfo and print an error message

	freeaddrinfo(result);

	if(ConnectSocket == INVALID_SOCKET){
		printf("Unable to connect to server!\n");
		WSACleanup();
		m_bRunning = false;
		m_bConnectionTried = true; // Connection failed, we tried..
		return -1;
	}
	m_bConnected = true;
	m_bConnectionTried = true; // Now we definitely tried, as we are connected!!!

	const int DEFAULT_BUFLEN = 1024;
	int recvbuflen = DEFAULT_BUFLEN;

	//char *sendbuf = "this is a test";
	char recvbuf[DEFAULT_BUFLEN];

	if(nullptr != m_pSender)
	{
		delete m_pSender;
		m_pSender = nullptr;
	}
    // Send an initial buffer
    m_pSocket = &ConnectSocket;

	m_pSender = new CSenderHandler(this);
	m_pSender->Start(); // Start sender!

	/*
	iResult = send(ConnectSocket, sendbuf, (int) strlen(sendbuf), 0);
	if(iResult == SOCKET_ERROR){
		printf("send failed: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		m_bRunning = false;
		return 1;
	}*/

	//printf("Bytes Sent: %ld\n", iResult);

	// shutdown the connection for sending since no more data will be sent
	// the client can still use the ConnectSocket for receiving data
	/*
	iResult = shutdown(ConnectSocket, SD_SEND);
	if(iResult == SOCKET_ERROR){
		printf("shutdown failed: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		m_bRunning = false;
		return 1;
	}*/

    SendFirstMessage(); // Lets send what we need! Then start receiving!
	// Receive data until the server closes the connection
	do
	{
		iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
		if(iResult > 0)
		{
			AddData(recvbuf, iResult);
		}
		else if(0 == iResult)
		{
			printf("Connection closed\n");
		}
		else
		{
			printf("recv failed: %d\n", WSAGetLastError());
		}
	} while(iResult > 0);

	// shutdown the send half of the connection since no more data will be sent
	iResult = shutdown(ConnectSocket, SD_SEND);
	if(iResult == SOCKET_ERROR){
		printf("shutdown failed: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		m_bRunning = false;
		return -1;
	}

	// cleanup
	closesocket(ConnectSocket);
	WSACleanup();
	m_bRunning = false;
	m_bConnected = false;
    return 1;
}
bool 
CNetworkClientConnection::IsConnected()
{
	return m_bConnected;
}
bool 
CNetworkClientConnection::IsConnectionTried()
{
	return m_bConnectionTried;
}
CNetworkMessage *
CNetworkClientConnection::GetMsg()
{
	return m_receiveMessages.Take();
}
bool 
CNetworkClientConnection::SendMsg(CNetworkMessage * pMsg)
{
	return m_sendMessages.Add(pMsg);
}
void 
CNetworkClientConnection::AddData(char * pBytes, int iLength)
{
	int iAllDataLength = iLength + (m_iLeftOversLength - m_iLeftOversOffset);
	char * pAllData = new char[iAllDataLength];
	if(m_iLeftOversLength > 0) // We need to enter this even if we don't copy anything, just to delete the pointer!
	{
		memcpy(pAllData, m_pLeftOvers + m_iLeftOversOffset, m_iLeftOversLength - m_iLeftOversOffset);
		delete m_pLeftOvers;
	}
	memcpy(pAllData + (m_iLeftOversLength - m_iLeftOversOffset), pBytes, iLength);
	m_iLeftOversOffset = 0;
	m_iLeftOversLength = 0;
	// Now we have all data in one array!
	
	m_pLeftOvers = pAllData;
	m_iLeftOversLength = iAllDataLength;

	int iMessageLength = -1;
	if(iAllDataLength >= 4)
	{
		iMessageLength = GetInt(pAllData) - 4; // AUGGMED requires length to be included in the length
	}
	else
	{
		return;
	}
	if(iMessageLength <= 0 || iMessageLength > 65000) // Current maximum message length!
	{
		Disconnect();
		return;
	}
	while(iMessageLength > 0 && iAllDataLength - 4 >= iMessageLength)
	{
		// We have enough for a message!
		char * pMsg = new char[iMessageLength];
		memcpy(pMsg, pAllData + 4, iMessageLength);
		m_receiveMessages.Add(new CNetworkMessage(pMsg, iMessageLength)); // We have a new message!
		int iTaken = iMessageLength + 4;
		m_iLeftOversOffset += iTaken;
		pAllData += iTaken;
		iAllDataLength -= iTaken;
		if(iAllDataLength > 4)
		{
			iMessageLength = GetInt(pAllData) - 4; // AUGGMED requires length to be included in the length
            if (iMessageLength > 1024)
            {
                OutputDebugString("WARNING: Network message is longer than 1024!");
            }
		}
		else
		{
			iMessageLength = -1; // We don't have enough data for determining length!
		}
	}
}
int 
CNetworkClientConnection::GetInt(char * pBytes)
{
	int i = ((int)(pBytes[0] & 0xFF) << 24);
	i += ((int)(pBytes[1] & 0xFF) << 16);
	i += ((int)(pBytes[2] & 0xFF) << 8);
	i += (int)(pBytes[3] & 0xFF);
	i = CEndianness::BigInt(i);
	return i;
}
void 
CNetworkClientConnection::IntToData(char * pBytes, int iValue)
{
	iValue = CEndianness::BigInt(iValue);
	pBytes[0] = (char)((iValue >> 24) & 0xFF);
	pBytes[1] = (char)((iValue >> 16) & 0xFF);
	pBytes[2] = (char)((iValue >> 8) & 0xFF);
	pBytes[3] = (char)(iValue & 0xFF);
}
int 
CNetworkClientConnection::Send()
{
	char lengthData[4];
	SOCKET ConnectSocket = *((SOCKET *)m_pSocket);
	while(m_bRunning && 0 != ConnectSocket)
	{
        CNetworkMessage * pMsg = nullptr;
		while(nullptr != (pMsg = m_sendMessages.Take()))
		{
			IntToData(lengthData, pMsg->Length() + 4); // AUGGMED requires the length to be inluded in the length
			int iResult = send(ConnectSocket, lengthData, 4, 0); // Send length first!
			iResult = send(ConnectSocket, pMsg->GetBytes(), pMsg->Length(), 0);
			if(iResult == SOCKET_ERROR){
				printf("send failed: %d\n", WSAGetLastError());
				closesocket(ConnectSocket);
				WSACleanup();
				m_bRunning = false;
				return 1;
			}
			delete pMsg;
		}
		Sleep(5); // Nothing in the queue, so lets sleep now
	}
	return 0;
}
void 
CNetworkClientConnection::SendFirstMessage()
{
    /*
    CNetworkClientMessage * pMsg = nullptr;
    //// EXMS_SCENARIO
    // ScenarioID (UINT) = 1
    // Random Value (UINT) = 0
    pMsg = new CNetworkClientMessage(CAuggmedMessageIds::EXMS_SCENARIO, 4 + 4 + 4);
    pMsg->Set(1, 4);
    pMsg->Set(0, 4 + 4);
    SendMsg(pMsg);

    //// EXMS_LOADFIRE
    // LocationId (UINT) = 13
    // FireStrength (Float)
    // Ret { EXMS_FIRELOADED
    pMsg = new CNetworkClientMessage(CAuggmedMessageIds::EXMS_LOADFIRE, 4 + 4 + 4);
    pMsg->Set(13, 4);
    pMsg->Set(4.0f, 4 + 4);
    SendMsg(pMsg);

    //// EXMS_FLOWRATE
    // FlowRatePPM (Float)
    pMsg = new CNetworkClientMessage(CAuggmedMessageIds::EXMS_FLOWRATE, 4 + 4);
    pMsg->Set(24.0f, 4);
    SendMsg(pMsg);

    //// EXMS_POPULATE
    // AvatarCount (UINT) = 400
    // Ret { EXMS_SIMULATION_READY
    pMsg = new CNetworkClientMessage(CAuggmedMessageIds::EXMS_POPULATE, 4 + 4);
    pMsg->Set(400, 4);
    SendMsg(pMsg);
    */
}