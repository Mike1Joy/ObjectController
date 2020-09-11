////////////////////////////////////////////////////////////////////////////////
//
// Simple exodus client
// Communication is carried out via the file system.
//
// Author P.J.Lawrence 2007
//
////////////////////////////////////////////////////////////////////////////////
#ifndef CLIENT_EXODUS_H
#define CLIENT_EXODUS_H
////////////////////////////////////////////////////////////////////////////////
typedef enum
{
    ECS_OK,
    ECS_FAILD_TO_INIT,
    ECS_FAILED_TO_CREATE_FILE,
    ECS_FAILED_CONNECTING_TO_THE_SERVER,
    ECS_FAILED_CREATING_THREAD,
    ECS_ALREADY_CONNECTED,
    ECS_SERVER_NOT_READY
} ExodusClientStatus_e;
////////////////////////////////////////////////////////////////////////////////

// Geometry   (10000)

// Population (20000)
const unsigned int EXMS_POP_FILE_CREATED       = 20001;
const unsigned int EXMS_EPF_FILE_CREATED       = 20002;
const unsigned int EXMS_EPB_FILE_CREATED       = 20003;

// Scenario   (30000)

// Simulation (40000)
const unsigned int EXMS_SIMULATION_START       = 4000;
const unsigned int EXMS_SIMULATION_PAUSE       = 4001;
const unsigned int EXMS_SIMULATION_CONTINUE    = 4002;
const unsigned int EXMS_SIMULATION_TERMINATE   = 4003;
const unsigned int EXMS_SIMULATION_RESET       = 4004;
const unsigned int EXMS_SIMULATION_INITIALISE  = 4005;
const unsigned int EXMS_SIMULATION_STEP        = 4006;
const unsigned int EXMS_SIMULATION_READY       = 4007; // ready*
const unsigned int EXMS_SMARTFIRE_LAUNCH       = 4008;
const unsigned int EXMS_EXODUS_LAUNCH          = 4009;
const unsigned int EXMS_LOAD_SMF_SCENARIO      = 4010;
const unsigned int EXMS_SMARTFIRE_HALT         = 4011;
const unsigned int EXMS_SMARTFIRE_CONTINUE     = 4012;
const unsigned int EXMS_SMARTFIRE_TERMINATE    = 4013;

// Progress
const unsigned int EXMS_SIMULATION_STARTED     = 4500; // playing*
const unsigned int EXMS_SIMULATION_PAUSED      = 4501; // pausing*
const unsigned int EXMS_SIMULATION_COMPLETED   = 4502;
const unsigned int EXMS_SIMULATION_PROGRESS    = 4503;
const unsigned int EXMS_SIMULATION_INITIALISED = 4504;
const unsigned int EXMS_SIM_FILE_CREATED       = 4554;

// EXODUS Batch files
// Batch Commands (5001)
const unsigned int EXMS_READY_FOR_EBF          = 5000;
const unsigned int EXMS_EXEC_EBF_FILE          = 5001;
const unsigned int EXMS_PAUSE_EBF_SCRIPT       = 5002;
const unsigned int EXMS_CONTINUE_EBF_SCRIPT    = 5003;
const unsigned int EXMS_TERMINATE_EBF_SCRIPT   = 5004;

// Batch Errors
const unsigned int EXMS_ERROR_PARSING_EBF_FILE        = 5050;
const unsigned int EXMS_ERROR_FAILED_TO_OPEN_EBF_FILE = 5051;

// Batch Status
const unsigned int EXMS_PARSING_EBF_FILE       = 5200;
const unsigned int EXMS_WAITING_FOR_EBF_DATA   = 5201;
const unsigned int EXMS_EXEC_EBF_COMPLETE      = 5202;

// EXODUS script files
// SFE Files      (5400)
const unsigned int EXMS_EXEC_SFE_FILE          = 5400;
const unsigned int EXMS_PAUSE_SFE_SCRIPT       = 5401;
const unsigned int EXMS_CONTINUE_SFE_SCRIPT    = 5402;
const unsigned int EXMS_TERMINATE_SFE_SCRIPT   = 5403;
const unsigned int EXMS_RESTART_SFE_SCRIPT     = 5404;

// SFE Errors
const unsigned int EXMS_ERROR_PARSING_SFE_FILE        = 5450;
const unsigned int EXMS_ERROR_FAILED_TO_OPEN_SFE_FILE = 5451;

// SFE Status
const unsigned int EXMS_READY_FOR_SFE          = 5501; /* Not Implemented */
const unsigned int EXMS_PARSING_SFE_FILE       = 5502;
const unsigned int EXMS_WAITING_FOR_SFE_DATA   = 5503;
const unsigned int EXMS_EXEC_SFE_COMPLETE      = 5504;
const unsigned int EXMS_EXEC_SFE_TEMINATED     = 5505;
const unsigned int EXMS_EXEC_SFE_RESTARTED     = 5506;

// EXODUS Scenario files
// SSF Files      (5600)
const unsigned int EXMS_PAUSE_SSF_SCRIPT       = 5601;
const unsigned int EXMS_CONTINUE_SSF_SCRIPT    = 5602;
const unsigned int EXMS_TERMINATE_SSF_SCRIPT   = 5603;
const unsigned int EXMS_RESTART_SSF_SCRIPT     = 5604;
//const unsigned int EXMS_EXEC_SFE_TEMINATED   = 5505;
//const unsigned int EXMS_EXEC_SFE_RESTARTED   = 5506;

// SSF Errors
const unsigned int EXMS_ERROR_PARSING_SSF_FILE        = 5650;
const unsigned int EXMS_ERROR_FAILED_TO_OPEN_SSF_FILE = 5651;

// SSF Status
const unsigned int EXMS_PARSING_SSF_FILE       = 5700;
const unsigned int EXMS_WAITING_FOR_SSF_DATA   = 5701;
const unsigned int EXMS_EXEC_SSF_COMPLETE      = 5702;
const unsigned int EXMS_EXEC_SSF_TEMINATED     = 5703;

// ESF Messages
const unsigned int EXMS_MESSAGE_STRING         = 6000;
const unsigned int EXMS_MSG_DOOR_EVENT         = 6001;
const unsigned int EXMS_MSG_FIRE_EVENT         = 6002;
const unsigned int EXMS_MSG_TRIGGER_EVENT      = 6003;
const unsigned int EXMS_MSG_HATCH_EVENT        = 6004;

// General errors and status
const unsigned int EXMS_EXODUSERROR            = 7000;

//General operations
const unsigned int EXMS_SHUTDOWN               = 7100;
const unsigned int EXMS_SHUTTING_DOWN          = 7101;
const unsigned int EXMS_HIDEINTERFACE          = 7102;

// Connection
const unsigned int EXMS_CONNECTING             = 8001;
const unsigned int EXMS_CONNECTED              = 8002;
const unsigned int EXMS_DISCONNECT             = 8003;
const unsigned int EXMS_DISCONNECTED           = 8004;
const unsigned int EXMS_SMF_CONNECTED          = 8005;
const unsigned int EXMS_SMF_DISCONNECTED       = 8006;
const unsigned int EXMS_SMF_DO_DEBUG           = 8007;

// Data file Connection
const unsigned int SMF_EX_DAT_FILE                    = 9020;
const unsigned int SMF_EX_DAT_READY                   = 9021;
const unsigned int EXMS_SMF_ESF_READY                 = 9022;
const unsigned int EXMS_SMF_ESF_TERMINATE             = 9023;
const unsigned int EXMS_SMF_DO_CONTINUE               = 9026;
const unsigned int EXMS_TO_SMF_HAZARDS_UPDATED        = 9027;
const unsigned int EXMS_START_OF_SMF_CHANGES          = 9028;
const unsigned int EXMS_END_OF_SMF_CHANGES            = 9029;
const unsigned int EXMS_START_OF_SMF_TIME_STEP        = 9030;
const unsigned int EXMS_WAITING_FOR_HAZARD_DATA       = 9031;
const unsigned int EXMS_CHANGES_AVAILABLE             = 9034;
const unsigned int EXMS_SMF_INITIALISATION_COMPLETE   = 9035;

// AUGGMED SIMULATION COMMAND IDs

const unsigned int EXMS_SCENARIO              = 10001; // scenario*
const unsigned int EXMS_SCENARIOLOADED        = 10002; // scenarioLoaded*
const unsigned int EXMS_SCENARIOFAILURE       = 10003; // scenarioFailure - NOT IN WIKI
const unsigned int EXMS_POPULATE              = 10004; // populate*
const unsigned int EXMS_FLOWRATE              = 10005; // flowrate*
const unsigned int EXMS_LOADFIRE              = 10006; // loadfire*
const unsigned int EXMS_FIRELOADED            = 10007; // fireLoaded*
const unsigned int EXMS_STARTFIRE             = 10008; // startFire*
const unsigned int EXMS_FIRESTATED            = 10009; // fireStarted*
const unsigned int EXMS_STARTALARM            = 10010; // startAlarm*
const unsigned int EXMS_ALARMSTARTED          = 10011; // alarmStarted*

const unsigned int EXMS_TIME                  = 10100; // time*
const unsigned int EXMS_ADD                   = 10101; // add*
const unsigned int EXMS_ADDED                 = 10102; // added*
const unsigned int EXMS_DELETE                = 10103; // delete*
const unsigned int EXMS_MOVE                  = 10104; // move**
const unsigned int EXMS_MOVED                 = 10105; 
const unsigned int EXMS_PERSONFACE            = 10106; // face*
const unsigned int EXMS_REMOVE                = 10107;
const unsigned int EXMS_ADDOBSTRUCTION        = 10108; // add obstruction
const unsigned int EXMS_OBSTRUCTIONADDED      = 10109; // obstruction added
const unsigned int EXMS_OBSTRUCTIONFAILURE    = 10110; // obstruction failure

const unsigned int EXMS_HAND                  = 10200;
const unsigned int EXMS_VOICE                 = 10201;
const unsigned int EXMS_GESTURE               = 10202; // gesture*
const unsigned int EXMS_WEAPON                = 10203;

const unsigned int EXMS_INJURE                = 10300;
const unsigned int EXMS_HAZARDQUERY           = 10301; // hazardQuery
const unsigned int EXMS_HAZARDSTATUS          = 10302; // hazardStatus

const unsigned int EXMS_KILL                  = 10401; // Kill**
const unsigned int EXMS_KILLED                = 10404; // Killed - NOT IN WIKI
const unsigned int EXMS_SHOT                  = 10402; // Shot
const unsigned int EXMS_TOXIT                 = 10403;

const unsigned int EXMS_PERSONTHREAT          = 10501; // personThreat
const unsigned int EXMS_STATICTHREAT          = 10502; // staticThreat
const unsigned int EXMS_BOMB                  = 10503; // bomb*
const unsigned int EXMS_BOMBDETONATED         = 10504; // bombDetonated*
const unsigned int EXMS_DAMAGE                = 10506; // damage*
const unsigned int EXMS_FBDUPDATE             = 10507; // fbdupdate - NOT IN WIKI

const unsigned int EXMS_JSON                  = 0x4A534F4E; // JSON Message
////////////////////////////////////////////////////////////////////////////////
class EXODUSTCPClient;
struct AgentClientData
{
    Person_Class *m_Person;
    UINT m_ID;
    float m_X,m_Y,m_Z;
    float m_Rotation;
    int m_iAction1;
    float m_Vx,m_Vy,m_Vz;
	float m_FBD;
	
    AgentClientData() {
        m_Person=nullptr;
        m_ID=0; 
        m_X=m_Y=m_Rotation=0.0f;
        m_iAction1=1;
        m_Vx=m_Vy=m_Vz=0.0f;
		m_FBD=0.0f;
    }
    AgentClientData(Person_Class* aPerson,UINT aID,float aX,float aY,float aZ,float aRotation): m_Person(aPerson), m_ID(aID), m_X(aX),m_Y(aY),m_Z(aZ),m_Rotation(aRotation) { m_iAction1=1; m_Vx=m_Vy=m_Vz=0.0f; m_FBD=0.0f; }
    //AgentClientData(Person_Class* aPerson,UINT aID,float aX,float aY,float aRotation, int iAction1): m_Person(aPerson), m_ID(aID), m_X(aX),m_Y(aY),m_Rotation(aRotation), m_iAction1(iAction1) { m_Vx=m_Vy=0.0f; }
    AgentClientData(Person_Class* aPerson,UINT aID,float aX,float aY,float aZ,float aVx,float aVy,float aVz,float aRotation, int iAction1, float aFBD): m_Person(aPerson), m_ID(aID), m_X(aX),m_Y(aY),m_Z(aZ),m_Rotation(aRotation), m_iAction1(iAction1),m_Vx(aVx),m_Vy(aVy),m_Vz(aVz),m_FBD(aFBD) {  }
};
////////////////////////////////////////////////////////////////////////////////
class UnityMessageQueue_Class;
////////////////////////////////////////////////////////////////////////////////
extern void GetCommsMessageName( unsigned int mess_code, char name_buff[] );
bool ConvertEXODUSToUnity(float &fX,float &fY,float &fZ);
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
class ExodusClient
{
private:
    int m_ErrorCode; // stores a system error code
    char *m_FileName; // server file name
    UINT m_PortID;    // server port
    char m_ServerHostName[255];
    bool m_Connected;
    bool m_TerminateScript;
    bool m_RestartScript;
    bool m_TerminateSimulation;
    time_t m_AccessTime;
    time_t m_LastReadTime;
    RemoteSystemTypes m_SystemConnectedTo;
    bool m_IsServer; // using unity connection
    CTCArray<AgentClientData*> *m_AgentClientDataList;

    bool m_SendSimulationData; // send data about agent locations and model
    bool m_SendExodusStatusMessages; // send data about agent locations and model
    
    unsigned int m_ExodusErrorCode; // stores exodus error code

    float m_LastSimTimeOutput;
    time_t m_LastSimulationProgressOutput;
    unsigned int m_SMFTimeStepNumber;
    bool m_SMFDebugRequest;

    EXODUSTCPClient* m_TCPUDPClient;
    
    AgentClientData *LocateEntry(Person_Class *aPerson,int aPersonID) const;
    bool CorrectAgentID(Person_Class *aPerson,int &aPersonID) const;

    friend UnityMessageQueue_Class;
    UnityMessageQueue_Class *m_UnityMessageQueue;
    
    int UnityRunTimeComms(char* const buf);
    int UnityReadComm(char* const,unsigned int &Code, unsigned int &Data);
public:
    bool FireUnityMessageQueue();

    ExodusClient();
    ~ExodusClient() { CloseConnection(); }
    ExodusClientStatus_e Connect();
    ExodusClientStatus_e ConnectSubThread();

    ExodusClientStatus_e ConnectClientToServer(); // function to allow the connection of more than one client
    
    // file base connection
    bool SetServerFile(const char* aFileName);
    const char* GetServerFile() { return (m_FileName); }

    // server based connection
    void SetServerPort(UINT aPort)  {m_PortID=aPort;}
    UINT GetServerPort() const      {return (m_PortID);}

    bool SetServerHostName( const char* serverHostName ) { return ( ( strcpy( m_ServerHostName, serverHostName ) ) ? true : false ); }
    const char* GetServerHostName() { return (m_ServerHostName); }

    // server based connection
    void SetRemotePort(unsigned short aPort);
    unsigned short GetRemotePort() const;
    int GetNumberOfConnections() const;

    // bool Unity connection
    RemoteSystemTypes GetConnectedSystemType() const { return(m_SystemConnectedTo); } 
    bool SetConnectedSystemType(RemoteSystemTypes aSystem) { m_SystemConnectedTo=aSystem; return(true); }
    
    void SetSendSimulationData(bool aType)  {m_SendSimulationData=aType;}
    bool CanSendSimulationData() const      {return (m_SendSimulationData);}

    void SetSendExodusStatusMessages(bool aType)  {m_SendExodusStatusMessages=aType;}
    bool CanSendExodusStatusMessages() const      {return (m_SendExodusStatusMessages);}
    
    // bool server connection
    void SetAsServer(bool aType)  {m_IsServer=aType;}
    bool IsServer() const         {return (m_IsServer);}

    void EnableDataSend(bool aStatus);
    
    int CloseConnection();

    bool SendUnityMessage(const unsigned int Code,const unsigned int Data);
    int SendData(const unsigned int Code,const unsigned int Data=0,int ConnectionID=0);
    int SendDataExtra(const unsigned int Code,const unsigned int Data=0,const unsigned int BufferSize=0,const char *DataBuffer=NULL,int IDConnection=0);
    int WaitForReply(unsigned int &Code, unsigned int &Data);
    int ReadCommTCPUDP(unsigned int &Code, unsigned int &Data, char* &ExtraData, size_t &ExtraDataSize,int IDConnection=0);
    int ReadComm(unsigned int &Code, unsigned int &Data, void * pMsg = NULL);
    int ReadAdditionalDataBlock(char * &DataBuffer, bool ClearMessageWhenRead=false);
    bool PostDataCheck();
    bool SetPerson(Person_Class *aPerson, int iStructure, int iLevel, float fX, float fY, float fZ, float fYRotation, float aSpeed, float fXSpeed, float fYSpeed, float fZSpeed, int iAction1, int iAction2, int iHazard, int iStartFrame,bool InSimulation,float AgentTime);
    bool SetDamage(ExFloat fX,ExFloat fY,ExFloat fZ,UINT aDamageLevel);
    bool SetDamage(UINT* The2DDamageArray,UINT MinRow,UINT MaxRow,UINT MinCol,UINT MaxCol,ExFloat TopLeftXPos,ExFloat TopLeftYPos,UINT NumberOfRows,ExFloat TheHeight);
    bool SetFrame(int iFrame);
    bool ClearAllPeople();
    bool RemoteAddedPerson(Person_Class *aPerson, int UnityID,float aBaseHeight);
    bool SendReadyCommand();
    bool SendTCPUDPData(const char* aDataString,bool ApplyDelay);
    UINT GetAgentClientDataListSize() const;

    bool IsConnected() const { return(m_Connected); }
    int GetErrorCode() const { return(m_ErrorCode); }
    bool IsTCPUDP() const { return(m_TCPUDPClient!=nullptr); }
    
    unsigned int GetExoErrorCode() { return(m_ExodusErrorCode); }
    void SetExoErrorCode(unsigned int aCode) { m_ExodusErrorCode=aCode; }

    unsigned int GetSMFTimeStepNumber() { return(m_SMFTimeStepNumber); }
    void SetSMFTimeStepNumber(unsigned int aSMFTSNum) { m_SMFTimeStepNumber=aSMFTSNum; }

    bool GetSMFDebugRequest() { return(m_SMFDebugRequest); }
    void SetSMFDebugRequest(bool aFlag) { m_SMFDebugRequest=aFlag; }

    bool TerminateScript() { return(m_TerminateScript); }
    void SetTerminateScript(bool aStatus) { m_TerminateScript=aStatus; }

    bool RestartScript() { return(m_RestartScript); }
    void SetRestartScript(bool aStatus) { m_RestartScript=aStatus; }

    float GetLastSimTimeOutput() const      { return(m_LastSimTimeOutput); }
    void  SetLastSimTimeOutput(float aTime) { m_LastSimTimeOutput=aTime; }

    time_t GetLastSimulationProgressOutput() const      { return(m_LastSimulationProgressOutput); }
    void  SetLastSimulationProgressOutput(time_t aTime) { m_LastSimulationProgressOutput=aTime; }

	// vrEXODUS server connection:
	bool ProcessPersonMovementTargetMsg(void * pMsg);

    // Smartfire connection
private:
    char *m_DynamicHazardFile; // server file name
    bool m_UseDynamicHazardFile;
    UINT m_TotalHazardDataTime; // How much hazard data will be provided in seconds
    float m_HazardFileCurrentTime; // current time step for hazard file
    volatile bool m_WaitingForData; // flag to indicate exodus is waiting for data
public:
    bool UseDynamicHazardFile() const                     { return(m_UseDynamicHazardFile); }
    const char* GetDynamicHazardFile() const              { return(m_DynamicHazardFile); }
    void ActivateDynamicHazardFile(bool aStatus)          { m_UseDynamicHazardFile=aStatus; }
    void SetDynamicHazardFile(const char* aFilename);
    UINT GetTotalHazardDataTime() const                   { return(m_TotalHazardDataTime); }
    void SetTotalHazardDataTime(UINT SecondsSimulated)    { m_TotalHazardDataTime=SecondsSimulated; }
    float GetHazardFileCurrentTime() const                { return(m_HazardFileCurrentTime); }
    void SetHazardFileCurrentTime(float SecondsSimulated) { m_HazardFileCurrentTime=SecondsSimulated; }

    bool IsWaitingForData() const         { return(m_WaitingForData); }
    void SetWaitingForData(bool aStatus ) { m_WaitingForData=aStatus; }

    // logging of door events
private:
  #if !defined(_MSC_VER) || _MSC_VER>1400
    std::string m_DoorEvents;
    std::string m_HatchEvents;
  #endif

    UINT m_DoorEventCount;
    UINT m_HatchEventCount;
public:
    void ClearDoorEvents();
    void ClearHatchEvents();
    bool AddDoorEvent(ExitNode_Class *anExit);
    bool AddHatchEvent(MegaNode_Class *aTransitNode);
    bool AnyDoorEvents() const  { return(m_DoorEventCount>0); }
    bool AnyHatchEvents() const  { return(m_HatchEventCount>0); }
    UINT DoorEventCount() const { return(m_DoorEventCount); }
    UINT HatchEventCount() const { return(m_HatchEventCount); }
    const char* DoorEventData() const {
      #if !defined(_MSC_VER) || _MSC_VER>1400
        return(m_DoorEvents.c_str()); 
      #else
        return (nullptr);
      #endif
    }
    const char* HatchEventData() const {
      #if !defined(_MSC_VER) || _MSC_VER>1400
        return(m_HatchEvents.c_str()); 
      #else
        return (nullptr);
      #endif
    }
};
////////////////////////////////////////////////////////////////////////////////
namespace ExodusClientInterface
{
    extern ExodusClient *n_ExodusClientInterface;
    bool MakeClient();
    bool WaitForHazardData();
    bool SendSMFChangeMessages();
    bool HandleSMFMessages( UINT aCode, UINT aData, bool & WaitForMessage );
//    bool WaitForContinue();
    bool CloseClient();
    bool ServerControl();
    bool CheckForServerTermination();
    bool SimulationOptions(const unsigned int &anOption);

    extern UINT TimerEvent_Flag;
    bool StartServerEventTimer(unsigned int Elapsed);
    bool EndServerEventTimer();
}
////////////////////////////////////////////////////////////////////////////////
#define SWAP_UINT16(x) (((x) >> 8) | ((x) << 8))
#define SWAP_UINT32(x) (((x) >> 24) | (((x) & 0x00FF0000) >> 8) | (((x) & 0x0000FF00) << 8) | ((x) << 24))
size_t ConvertByteAndCopyOver(char *ResponseStr,UINT ResponseStringPos,UINT aValue);
size_t ConvertByteAndCopyOver(char *ResponseStr,UINT ResponseStringPos,int aValue);
size_t ConvertByteAndCopyOver(char *ResponseStr,UINT ResponseStringPos,float aValue);
size_t ConvertByteAndCopyOver8Bits(char *ResponseStr,UINT ResponseStringPos,UINT aValue);
size_t ConvertByteAndCopyOver8Bits(char *ResponseStr,UINT ResponseStringPos,char aValue);
void SWAP_FLOAT32(float &aValue);
#endif