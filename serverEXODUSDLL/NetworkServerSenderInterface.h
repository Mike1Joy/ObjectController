// Author/editor/coder: Simo
#pragma once
#include "Defines.h"
//#include "NetworkServerHandler.h"
//#include "GeometryHandler.h"
class CNetworkServerHandler;
class EXODUS_SERVER_DLL_API CNetworkServerSenderInterface
{
	friend class CNetworkServerHandler;
public:
	static void SetPerson(int iPersonId, int iStructure, int iLevel, float fX, float fY, float fZ, float fYRotation, float fXSpeed, float fYSpeed, float fZSpeed, int iAction1, int iAction2, int iHazard, int iStartFrame);
	static void SetWall(int iWallId, int iStructure, int iLevel, float fX, float fZ, float fHeight, float fLength, float fYRotation);
	static void SetLevel(int iLevelId, int iStructure, float fHeight);
	static void SetRegion(int iRegionId, int iStructure, int iLevel, float fX, float fZ, float fXLength, float fZLength, int iVertices, float * fXVertices, float * fZVertices);
	static void RemovePerson(int iPersonId);
	static void RemoveWall(int iWallId);
	static void RemoveLevel(int iLevelId);

	static void SetFrame(int iFrame);
	static void SetPeopleOutCount(int iPeopleOut);
	static void SetPeopleDeadCount(int iPeopleDead);

private:
	CNetworkServerSenderInterface(void);
	~CNetworkServerSenderInterface(void);
	//CGeometryHandler * m_pGeometry;
	//CNetworkHandler * m_pHandler;
};

