// Author/editor/coder: Simo
#pragma once
#include "ArrayList.h"
#include "NetworkMessage.h"
#include <tuple>
class CMessageClientParser
{
public:
	/*
	static bool GetLevelCount(CNetworkClientMessage * pMsg);
	static bool RetLevelCount(CNetworkClientMessage * pMsg, int &iLevelCount);
	static bool GetLevel(CNetworkClientMessage * pMsg, int &iLevel);
	static bool RetLevel(CNetworkClientMessage * pMsg, int &iLevel, int &iLevelHeight);
	static bool GetWallCount(CNetworkClientMessage * pMsg, int &iLevel);
	static bool RetWallCount(CNetworkClientMessage * pMsg, int &iLevel, int &iCount);
	static bool GetWall(CNetworkClientMessage * pMsg, int &iLevel, int &iWall);
	static bool RetWall(CNetworkClientMessage * pMsg, int &iLevel, int &iWall, float &fX, float &fZ, float &fLength, float &fRotation, float &fHeight);
	*/
	static bool SetLevelHeight(CNetworkMessage * pMsg, int &iStructureId, int &iLevelId, float &fLevelHeight);
	static bool SetWall(CNetworkMessage * pMsg, int &iStructureId, int &iLevelId, int &iWallId, float &fX, float &fZ, float &fLength, float &fRotation, float &fHeight);
	static bool SetPerson(CNetworkMessage * pMsg, int &iPersonId, int &iType, int &iAge, int &iVestColour, int &iEthnicity);
	//static bool SetPersonAction(CNetworkClientMessage * pMsg, int &iPersonId, int &iFrame, float &fXPosition, float &fYPosition, float &fZPosition, int &iLevel, int &iAction1, int &iAction2, float &fXSpeed, float &fYSpeed, float &fZSpeed, float &fYRotation, int &iHazard);
    /*
	static bool SetPersonAction(CNetworkClientMessage * pMsg, int &iFrame, CArrayList<CPersonUpdate> * pUpdateList);
	static CNetworkPerson * SetPerson(CNetworkClientMessage * pMsg);
    static CNetworkPerson * SetPersonAuggmed(CNetworkClientMessage * pMsg);
    */
    static int SetTimeAuggmed(CNetworkMessage * pMsg);
    static std::tuple<int, int> SetPersonAddedAuggmed(CNetworkMessage * pMsg);
    /*
	static CNetworkWall * SetWall(CNetworkClientMessage * pMsg);
	static CNetworkLevel * SetLevel(CNetworkClientMessage * pMsg);
    */

	static int SetFrame(CNetworkMessage * pMsg);
	static bool SetPersonMovementTarget(CNetworkMessage * pMsg, int &iPersonId, float &fX, float &fZ, int &iStructure, int &iLevel);

	// Auxiliary class with static only functions doesn't need to be constructed
private:
	CMessageClientParser(void);
	~CMessageClientParser(void);
};

