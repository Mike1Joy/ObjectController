// Author/editor/coder: Simo
#include "NetworkServerHandler.h"
#include "NetworkServerSenderInterface.h"
#include "MessageServerComposer.h"


CNetworkServerSenderInterface::CNetworkServerSenderInterface(void)
{
}

CNetworkServerSenderInterface::~CNetworkServerSenderInterface(void)
{
}

void 
CNetworkServerSenderInterface::SetPerson(int iPersonId, int iStructure, int iLevel, float fX, float fY, float fZ, float fYRotation, float fXSpeed, float fYSpeed, float fZSpeed, int iAction1, int iAction2, int iHazard, int iStartFrame)
{
    /*
	CGeometryHandler * pGeometry = CNetworkServerHandler::GetGeometry();
	if(NULL != pGeometry)
	{
		CNetworkPerson * pNewPerson = new CNetworkPerson();
		pNewPerson->SetId(iPersonId);
		pNewPerson->SetStructureId(iStructure);
		pNewPerson->SetLevelId(iLevel);
		pNewPerson->SetXPosition(fX);
		pNewPerson->SetYPosition(fY);
		pNewPerson->SetZPosition(fZ);
		pNewPerson->SetYRotation(fYRotation);
		pNewPerson->SetXSpeed(fXSpeed);
		pNewPerson->SetYSpeed(fYSpeed);
		pNewPerson->SetZSpeed(fZSpeed);
		pNewPerson->SetAction1(iAction1);
		pNewPerson->SetAction2(iAction2);
		pNewPerson->SetHazard(iHazard);
		pNewPerson->SetFrame(iStartFrame);
		pGeometry->AddPersonUpdate(pNewPerson);
	}
    */
}
void 
CNetworkServerSenderInterface::SetWall(int iWallId, int iStructure, int iLevel, float fX, float fZ, float fHeight, float fLength, float fYRotation)
{
    /*
	CGeometryHandler * pGeometry = CNetworkServerHandler::GetGeometry();
	if(NULL != pGeometry)
	{
		CNetworkWall * pNewWall = new CNetworkWall();
		pNewWall->SetId(iWallId);
		pNewWall->SetStructureId(iStructure);
		pNewWall->SetLevelId(iLevel);
		pNewWall->SetX(fX);
		pNewWall->SetZ(fZ);
		pNewWall->SetHeight(fHeight);
		pNewWall->SetLength(fLength);
		pNewWall->SetRotation(fYRotation);
		pGeometry->AddWallUpdate(pNewWall);
	}
    */
}
void 
CNetworkServerSenderInterface::SetLevel(int iLevelId, int iStructure, float fHeight)
{
    /*
	CGeometryHandler * pGeometry = CNetworkServerHandler::GetGeometry();
	if(NULL != pGeometry)
	{
		CNetworkLevel * pNewLevel = new CNetworkLevel();
		pNewLevel->SetId(iLevelId);
		pNewLevel->SetStructureId(iStructure);
		pNewLevel->SetHeight(fHeight);
		pGeometry->AddLevelUpdate(pNewLevel);
	}
    */
}
void 
CNetworkServerSenderInterface::SetRegion(int iRegionId, int iStructure, int iLevel, float fX, float fZ, float fXLength, float fZLength, int iVertices, float * fXVertices, float * fZVertices)
{
	// Not implemented yet!
}
void 
CNetworkServerSenderInterface::RemovePerson(int iPersonId)
{
}
void 
CNetworkServerSenderInterface::RemoveWall(int iWallId)
{
}
void 
CNetworkServerSenderInterface::RemoveLevel(int iLevelId)
{
}

void 
CNetworkServerSenderInterface::SetFrame(int iFrame)
{
    CNetworkMessage * pMsg = CMessageServerComposer::SetFrame(iFrame);
	CNetworkServerHandler::BroadcastMessage(pMsg);
}
void 
CNetworkServerSenderInterface::SetPeopleOutCount(int iPeopleOut)
{
}
void 
CNetworkServerSenderInterface::SetPeopleDeadCount(int iPeopleDead)
{
}