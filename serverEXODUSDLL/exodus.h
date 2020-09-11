////////////////////////////////////////////////////////////////////////////////
#if _MSC_VER>1400
  #define __USE__WINSOCK2__
#endif
  #ifdef __USE__WINSOCK2__
    #include <winsock2.h>
    #include <ws2tcpip.h>
  #else
    #include <winsock.h>
  #endif
#include <windows.h>
//#define DOS
#include <stdio.h>
#include <new.h>
#include "math.h"
#include <string>
using namespace std;
#define ExoCONTINUOUS

#define _FileStatFunc _stat
#define EXODUS_snprintf _snprintf
#define _new_nothrow new(std::nothrow)

#define OutputStringSize 1022
extern char OutputString[OutputStringSize];

#define TheTol        0.0001f
#define DTheTol       0.00000001f

inline void SetToolBarText(const char *fmt, ...)
{
}
inline void output_text( char *the_text,FILE* outfile=nullptr)
{
    if (outfile) 
	{
		fprintf(outfile,"%s\n",the_text);
	} else {
		
	}
}
inline unsigned int GetCurrentTimeInMilliseconds()
{
	// This is all fine and dandy, but according to this:
	// http://msdn.microsoft.com/en-gb/library/windows/desktop/dd757629(v=vs.85).aspx
	// the accuracy may be less than 5 milliseconds or more. On Darren's and David's computer the accuracy is exactly 10 milliseconds
	// which isn't sufficient in controlling simulation speed to higher than 8.33 times real time (1000/120). Simo (02/09/2014)
	return 0;
}
//////////////////////////////////////////////////////////////////////
//
// Test routines to make initial connection
//
extern void StaticWriteLineToWindow(const char* SomeText);
extern bool StaticAddconnection(int anID);
extern void formated_output(const char *fmt, ...);
//////////////////////////////////////////////////////////////////////
inline void ExodusSleepFunction(unsigned int seconds)
{
    Sleep (seconds*1000);
}
//////////////////////////////////////////////////////////////////////
extern BOOL ProcessWaitingMsg();
////////////////////////////////
typedef float ExFloat;

#define ExodusTimeStep 12
#define ExodusTimeIncrement 0.08333333f

enum RemoteSystemTypes
{
    unknown_interface=0,
    vrEXODUS_Interface,
    UNITY_AUGGMED_Interface,
    SMARTFIRE_interface,
    DSTLauncher_interface
};

#define __Bomb_Default_Grid_Size__ 0.5f
#define __Bomb_Array_Size__ 4

#define GeometryMode             0
#define PopulationMode           1
#define ScenarioMode             2
#define SimulationMode           3
#define SimulationModeRun        4

class ExodusWorkSpace
{
private:
    float m_SimulationTime;
    bool m_SimPaused;
    UINT m_ProgramMode;
    bool m_StopSimulation;
public:
    ExodusWorkSpace() {
        m_SimulationTime=0.0f;
        m_SimPaused=false;
        m_StopSimulation=false;
    }
    UINT          GetProgramMode    () const { return(m_ProgramMode); }
    UINT GetAssembledCount() { return(0); }

    float GetSimulationTime() const { return(m_SimulationTime); }
    bool StopOperationSimulation() const {return(m_StopSimulation); }
    bool  IsPaused() const { return (m_SimPaused); }
    bool NeedToResetSimulation() { return(true); }
    void SimulationMenuPause(BOOL PauseSim=TRUE) { m_SimPaused=(PauseSim==TRUE);}
    void SimulationMenuReset() { m_SimulationTime=0.0f; }
    void DoSimEnd() {  }
    void SimulationMenuInitialiseRun() { }
    void SimulationMenuRunSimulation() { }
    bool DoOneStep() {m_SimulationTime+=ExodusTimeIncrement; return(true);}

    bool IsUpdatingUserInterface() const { return(false); }
};
/////////////////////////////////////////////////
extern ExodusWorkSpace *g_WorkSpace;
/////////////////////////////////////////////////
inline BOOL StopOperation() { return(FALSE); }
inline BOOL ShowInterface(BOOL aStatus) { return(FALSE); }
/////////////////////////////////////////////////
#define PERCOL_DEFAULT   0
#define PERCOL_RED       1
#define PERCOL_GREEN     2
#define PERCOL_DKBLUE    3
#define PERCOL_BLUE      4
#define PERCOL_CYAN      5
#define PERCOL_DKMAGENTA 6
#define PERCOL_MAGENTA   7
#define PERCOL_YELLOW    8
#define PERCOL_DKGRAY    9
#define PERCOL_GRAY     10
class Exodus_Object
{
private:
    UINT m_Label; // internal label used for identifying objects
public:
    // Constructor
    Exodus_Object(UINT aLabel=0) { m_Label=aLabel; }
    Exodus_Object(Exodus_Object * aExodusObject) {  m_Label=aExodusObject->m_Label; }
    
    UINT GetLabel() const       { return (m_Label); }
    void SetLabel(UINT ALabel)  { m_Label=ALabel;   }
};
/////////////////////////////////////////////////
class Person_Class : public Exodus_Object
{
private:
    UINT m_Label;
    ExFloat m_X,m_Y;
    short int m_AssignedColour;
public:
    Person_Class(): Exodus_Object() {
        m_X=m_Y=0.0f;
        m_AssignedColour=0;
    }
    
    ExFloat GetExoX() const { return (m_X); }
    ExFloat GetExoY() const { return (m_Y); }
    void SetX(ExFloat aX)         { m_X = aX;}
    void SetY(ExFloat aY)         { m_Y = aY;}

    void SetAssignedColour(short int ACol) { m_AssignedColour = ACol; }

    BOOL isAlive() const { return(TRUE); }
    BOOL HighToxicityExposure() const { return(FALSE); }
    BOOL HasExited() const { return(FALSE); }
    BOOL HasExpired() const { return(FALSE); }
};
class ExitNode_Class : public Exodus_Object
{
private:
    char * m_TheTitle;
public:
    ExitNode_Class() : Exodus_Object() {
        m_TheTitle=nullptr;
    }
    char * GetTitle() const { return m_TheTitle; }
    void SetTitle(const char* aTitle);
    void CatTitle(const char * ExtTitle);

    bool isOpen()        const { return(true);     }
    bool isClosed()      const { return(false); }
    UINT isDoorBeingOpened() const { return (false);}
    UINT isDoorBeingClosed() const { return (false); }
};
class MegaNode_Class
{
public:
    MegaNode_Class() {
    }
};