// Author/editor/coder: Simo
#include <winsock2.h>
#include <Windows.h>
#include <WS2tcpip.h>
#include <IPHlpApi.h>
#include <stdio.h>
#include <iostream>

#pragma comment(lib, "Ws2_32.lib")

// #pragma comment(lib, "Mswsock.lib")

#include "Endianness.h"
#include "NetworkServerConnection.h"


CNetworkServerConnection::CNetworkServerConnection(void * pSocket)
	: CAbstractThread()
{
	m_pSocket = pSocket;
	m_bConnected = true;
	//m_bRunning = false;
	m_pLeftOvers = NULL;
	m_iLeftOversLength = 0;
	m_iLeftOversOffset = 0;
	m_pSender = new CSenderHandler(this);
	m_bRunningSend = false;
	m_bRunningReceive = false;
	m_bReceiveFinished = false;
	m_pSender->Start();
	Start(); // Start this connection off!
}


CNetworkServerConnection::~CNetworkServerConnection(void)
{
	delete m_pSender;
}

bool 
CNetworkServerConnection::Disconnect()
{
	if(!m_bConnected)
	{
		return false;
	}
	// cleanup
	if(NULL != m_pSocket)
	{
		SOCKET ConnectSocket = *((SOCKET *)m_pSocket);
		closesocket(ConnectSocket);
	}
	//WSACleanup();
	return true;
}
bool 
CNetworkServerConnection::IsConnected()
{
	return m_bConnected;
}
int 
CNetworkServerConnection::Run()
{
	if(m_bRunningReceive)
	{
		return 0;
	}
	m_bRunningReceive = true;
	Receive();
	m_bRunningReceive = false;
	m_bReceiveFinished = true;
	return 0;
}
void 
CNetworkServerConnection::Receive()
{
	if(NULL == m_pSocket)
	{
		m_bConnected = false;
		return;
	}
	int iResult = 0;
	
	SOCKET ClientSocket;
	ClientSocket = *((SOCKET *)m_pSocket);

	// Receive and Send Data on the Server
	const int DEFAULT_BUFLEN = 1024;
	char recvbuf[DEFAULT_BUFLEN];
	//int iSendResult;
	int recvbuflen = DEFAULT_BUFLEN;

	// Receive until the peer shuts down the connection
	do {
		iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
		if(iResult > 0){
			/*
			printf("Bytes received: %d\n", iResult);

			// Echo the buffer back to the sender
			iSendResult = send(ClientSocket, recvbuf, iResult, 0);
			if(iSendResult == SOCKET_ERROR){
				printf("send failed: %d\n", WSAGetLastError());
				closesocket(ClientSocket);
				//WSACleanup();
				m_bConnected = false;
				return;
			}
			printf("Bytes sent: %d\n", iSendResult);
			*/
			AddData(recvbuf, iResult);
		} else if(iResult == 0){
			printf("Connection closing...\n");
		} else{
			printf("recv failed: %d\n", WSAGetLastError());
			//closesocket(ClientSocket);
			//WSACleanup();
			//m_bConnected = false;
			//return;
		}
	} while(iResult > 0);

	// Disconnecting the Server
	// shutdown the send half of the connection since no more data will be sent
	iResult = shutdown(ClientSocket, SD_SEND);
	if(iResult == SOCKET_ERROR){
		printf("shutdown failed: %d\n", WSAGetLastError());
		closesocket(ClientSocket);
		//WSACleanup();
		return;
	}
	// cleanup
	closesocket(ClientSocket);
	m_bConnected = false;
}
CNetworkMessage *
CNetworkServerConnection::GetMsg()
{
	return m_receiveMessages.Take();
}
bool 
CNetworkServerConnection::SendMsg(CNetworkMessage * pMsg)
{
	return m_sendMessages.Add(pMsg);
}
void 
CNetworkServerConnection::AddData(char * pBytes, int iLength)
{
    int iAllDataLength = iLength + (m_iLeftOversLength - m_iLeftOversOffset);
    char * pAllData = new char[iAllDataLength];
    if (m_iLeftOversLength > 0) // We need to enter this even if we don't copy anything, just to delete the pointer!
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
    if (iAllDataLength >= 4)
    {
        iMessageLength = GetInt(pAllData) - 4; // AUGGMED requires length to be included in the length
    }
    else
    {
        return;
    }
    if (iMessageLength <= 0 || iMessageLength > 65000) // Current maximum message length!
    {
        Disconnect();
        return;
    }
    while (iMessageLength > 0 && iAllDataLength - 4 >= iMessageLength)
    {
        // We have enough for a message!
        char * pMsg = new char[iMessageLength];
        memcpy(pMsg, pAllData + 4, iMessageLength);
        m_receiveMessages.Add(new CNetworkMessage(pMsg, iMessageLength)); // We have a new message!
        int iTaken = iMessageLength + 4;
        m_iLeftOversOffset += iTaken;
        pAllData += iTaken;
        iAllDataLength -= iTaken;
        if (iAllDataLength > 4)
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
CNetworkServerConnection::GetInt(char * pBytes)
{
	int i = ((int)(pBytes[0] & 0xFF) << 24);
	i += ((int)(pBytes[1] & 0xFF) << 16);
	i += ((int)(pBytes[2] & 0xFF) << 8);
	i += (int)(pBytes[3] & 0xFF);
	i = CEndianness::BigInt(i);
	return i;
}
void 
CNetworkServerConnection::IntToData(char * pBytes, int iValue)
{
	iValue = CEndianness::BigInt(iValue);
	pBytes[0] = (char)((iValue >> 24) & 0xFF);
	pBytes[1] = (char)((iValue >> 16) & 0xFF);
	pBytes[2] = (char)((iValue >> 8) & 0xFF);
	pBytes[3] = (char)(iValue & 0xFF);
}
int 
CNetworkServerConnection::Send()
{
	if(m_bRunningSend){
		return 0;
	}
	m_bRunningSend = true;
	char lengthData[4];
	SOCKET ConnectSocket = *((SOCKET *)m_pSocket);
	while(m_bRunningSend && NULL != ConnectSocket && !m_bReceiveFinished)
	{
        CNetworkMessage * pMsg = NULL;
		while(NULL != (pMsg = m_sendMessages.Take()))
		{
            IntToData(lengthData, pMsg->Length() + 4); // AUGGMED requires the length to be inluded in the length
            int iResult = send(ConnectSocket, lengthData, 4, 0); // Send length first!
            iResult = send(ConnectSocket, pMsg->GetBytes(), pMsg->Length(), 0);
            if (iResult == SOCKET_ERROR) {
                printf("send failed: %d\n", WSAGetLastError());
                closesocket(ConnectSocket);
				//WSACleanup();
				m_bRunningSend = false;
				return 1;
			}
			delete pMsg;
		}
		Sleep(5); // Nothing in the queue, so lets sleep now
	}
	return 0;
}