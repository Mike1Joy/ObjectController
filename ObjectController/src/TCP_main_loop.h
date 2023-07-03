#pragma once
#include <MMSystem.h>

#include "NetworkServerHandler.h"
#include "NetworkClientHandler.h"

#include "DataStructs.h"
#include "ObjectSpace.h"
#include "Log.h"
#include "opengl.h"
#include "FileIO.h"

using namespace data_for_TCP;

static bool tcp_log_min = false;

//// Message Types
/// meta
constexpr UINT EXMS_SIMULATION_PAUSED = 4001;
constexpr UINT EXMS_SIMULATION_RESET = 4004;
constexpr UINT EXMS_SIMULATION_STARTED = 4500;
constexpr UINT EXMS_SIMULATION_COMPLETED = 4502;
constexpr UINT EXMS_EXEC_SFE_COMPLETE = 5504; // sends when all commands from sfe file have finished - use this to generate freespace
constexpr UINT EXMS_DISCONNECTED = 8004;
constexpr UINT EXMS_EXODUSERROR = 7000;
constexpr UINT EXMS_PARSING_SFE_FILE = 5502;
constexpr UINT EXMS_SIMULATION_PROGRESS = 4503;
/// setup
constexpr UINT EXMS_ADD_WALL = 10050;
constexpr UINT EXMS_SEND_NODE = 10051; // <NodeID><iNodeType><iObstacle><iLevel><sX><sY><sZ><floorHeight>
constexpr UINT EXMS_SEND_ARC = 10052; // <iArc><iNode1><iNode2><Length><fObstacle><fLane1><iPos1><fLane2><iPos2>
constexpr UINT EXMS_SEND_TRANSIT = 10053; // < iNodeID > <iNodeType><fLenght><fAngle><fHeight><iLanes><iSteps><fRiserHeight><fTreadDepth>
constexpr UINT EXMS_SEND_OBJECT = 10055; // <iObjectId><iType><iLevel><fAngle><sX><sY><sZ>
constexpr UINT EXMS_ADD_HEPTAD = 10111; // <iPersonId><aPET><NodeID><aDrive><anAgility><ObjectID><AttachmentPoint>
constexpr UINT EXMS_STAIR_NODE = 10056; // <iNodeID><iStairID><iLane><iStep> 
/// simulation receive
constexpr UINT EXMS_TIME = 10100; // < fTime >
constexpr UINT EXMS_DELETE = 10103; // < iPersonId >
constexpr UINT EXMS_MOVE_HEPTAD = 10112; // <iPersonId><aPET><NodeID><aTSA><aDrive><anAgility><ObjectID><AttachmentPoint>
constexpr UINT EXMS_ADDOBJECT = 10521; // <iObjectId><iObjectType><iLevel><fxpos><fypos><fzpos><frotation><fspeed>
constexpr UINT EXMS_REMOVEOBJECT = 10522; // <iObjectId>
constexpr UINT EXMS_TARGETOBJECT = 10530; // < iObjectId > <iNodeID><iLevel><fxpos><fypos><fzpos><fSimulationTime><PersonID>
constexpr UINT EXMS_ATTACHOBJECTPERSON = 10540; // < iObjectId > <iPersonId><iLocation>
constexpr UINT EXMS_RELEASEOBJECTPERSON = 10541; // < iObjectId > <iPersonId><iNodeID>
constexpr UINT EXMS_WAITING_FOR_OBJECT_DATA = 10550; // EX has finished its turn
/// simulation send
constexpr UINT EXMS_UNDEREXTERNALCONTROL = 10527; // <objectID><1 or 0> (1 = OC controlling it, 0 gives control to EXODUS)
constexpr UINT EXMS_MOVEOBJECT = 10520; // <iObjectId><iObjectType><iLevel><fxpos><fypos><fzpos><frotation><fspeed><iTaskStatus>
// iTaskStatus = 0 (only moved), 1 (only completed task), 2 (both moved and completed task)
constexpr UINT EXMS_VELOCITYOBSTACLE = 10526; // <uPersonID><uNodeCount>(<uNodeID1><fCost1>,...)
constexpr UINT EXMS_END_OF_OBJECT_DATA = 10552; // OC has finished its turn
constexpr UINT EXMS_NODEOBJECT = 10524; // <uObjectID><uNodeCount>(<uNodeID1>,...)
/// unused
//constexpr UINT EXMS_ERROR_PARSING_SFE_FILE = 5450; // < iLineNumber >
//constexpr UINT EXMS_ERROR_FAILED_TO_OPEN_SFE_FILE = 5451;
//constexpr UINT EXMS_PARSING_SFE_FILE = 5502;
//constexpr UINT EXMS_END_OF_OBJECT_RUN = 10553;
//constexpr UINT EXMS_SEND_NODE_NAME = 10054; // < NodiD > <iNodeID><StrLen><String>

// Type code map
std::map<UINT, msg_str> type_code = { 
//  {UNIT code, {char* code_name, {is_int, is_int, ...}}},
	{EXMS_SIMULATION_PAUSED, {"EXMS_SIMULATION_PAUSED", {}}},
	{EXMS_SIMULATION_RESET, {"EXMS_SIMULATION_RESET", {}}},
	{EXMS_SIMULATION_STARTED, {"EXMS_SIMULATION_STARTED", {}}},
	{EXMS_SIMULATION_COMPLETED, {"EXMS_SIMULATION_COMPLETED", {}}},
	{EXMS_EXEC_SFE_COMPLETE, {"EXMS_EXEC_SFE_COMPLETE", {}}},
	{EXMS_DISCONNECTED, {"EXMS_DISCONNECTED", {}}},
	{EXMS_ADD_WALL, {"EXMS_ADD_WALL", {true, true, false, false, false, false, false, false, false}}},
	{EXMS_SEND_NODE, {"EXMS_SEND_NODE", {true, true, true, true, false, false, false, false}}},
	{EXMS_SEND_ARC, {"EXMS_SEND_ARC", {true, true, true, false, false, false, true, false, true}}},
	{EXMS_SEND_TRANSIT, {"EXMS_SEND_TRANSIT", {true, true, false, false, false, true, true, false, false}}},
	{EXMS_SEND_OBJECT, {"EXMS_SEND_OBJECT", {true, true, true, false, false, false, false}}},
	{EXMS_ADD_HEPTAD, {"EXMS_ADD_HEPTAD", {true, false, true, false, false, true, true}}},
	{EXMS_STAIR_NODE, {"EXMS_STAIR_NODE", {true, true, true, true}}},
	{EXMS_TIME, {"EXMS_TIME", {false}}},
	{EXMS_DELETE, {"EXMS_DELETE", {true}}},
	{EXMS_MOVE_HEPTAD, {"EXMS_MOVE_HEPTAD", {true, false, true, false, false, false, true, true}}},
	{EXMS_ADDOBJECT, {"EXMS_ADDOBJECT", {true, true, true, false, false, false, false, false}}},
	{EXMS_REMOVEOBJECT, {"EXMS_REMOVEOBJECT", {true}}},
	{EXMS_TARGETOBJECT, {"EXMS_TARGETOBJECT", {true, true, true, false, false, false, false, true}}},
	{EXMS_ATTACHOBJECTPERSON, {"EXMS_ATTACHOBJECTPERSON", {true, true, true}}},
	{EXMS_RELEASEOBJECTPERSON, {"EXMS_RELEASEOBJECTPERSON", {true, true, true}}},
	{EXMS_WAITING_FOR_OBJECT_DATA, {"EXMS_WAITING_FOR_OBJECT_DATA", {}}},
	{EXMS_UNDEREXTERNALCONTROL, {"EXMS_UNDEREXTERNALCONTROL", {true, true}}},
	{EXMS_MOVEOBJECT, {"EXMS_MOVEOBJECT", {true, true, true, false, false, false, false, false, true}}},
	{EXMS_VELOCITYOBSTACLE, {"EXMS_VELOCITYOBSTACLE", {true, true}, {true, false}}},
	{EXMS_END_OF_OBJECT_DATA, {"EXMS_END_OF_OBJECT_DATA", {}}},
	{EXMS_NODEOBJECT, {"EXMS_NODEOBJECT", {true, true}, {true}}},
	{EXMS_EXODUSERROR, {"EXMS_EXODUSERROR", {true, true}}},
	{EXMS_PARSING_SFE_FILE, {"EXMS_PARSING_SFE_FILE", {true}}},
	{EXMS_SIMULATION_PROGRESS, {"EXMS_SIMULATION_PROGRESS", {true}}}
};
msg_str unrecognised_msg_code;
msg_str* get_type_code(UINT code)
{
	auto it = type_code.find(code);
	if (it != type_code.end())
	{
		return &(it->second);
	}
	else
	{
		return &unrecognised_msg_code;
	}
}

//// Constants
/// node types
#define EX_NODE_FREESPACE 0
#define EX_NODE_LANDING 6
#define EX_NODE_TRANSIT 80
#define EX_NODE_EXIT 100
/// simulation
constexpr UINT TIME_OUT = 60000; // ms until EXODUS_run_loop will timeout and return false

//// Vars
constexpr bool TCP_print_only = false;
static int client_id = 0;
static bool accept_con = false;
static bool simulating = false;
static bool Log_to_file = false;
static bool first_print_to_file = true;
static int msg_counter_sent = 0;
static int msg_counter_recv = -1; // Exodus double sends the first message, so -1 to keep Seq in sync
static int tcp_con_time = 0;

//// TCP Functions
bool StaticAddconnection(int anID)
{
	log_TCP.print("Client added with id %d", anID);
	client_id = anID;
	return true;
}
void formated_output(const char *fmt, ...)
{
	log_TCP.print();
	printf(fmt);
	printf("\n");
}
void StaticWriteLineToWindow(const char* SomeText)
{
	log_TCP.print(SomeText);
}
DWORD WINAPI timeGetTime()
{
	return GetTickCount();
}

//// Unrecognised Messages
struct message_basic
{
	int type;
	int length;
	int num_instances;

	message_basic(CNetworkMessage* msg) : type(msg->GetType()), length(msg->Length()), num_instances(1) {}

	bool operator== (const message_basic& other) const
	{
		return (type == other.type);
	}

	bool operator== (CNetworkMessage* msg) const
	{
		return (type == msg->GetType());
	}

	void operator++ ()
	{
		++num_instances;
	}

	void print()
	{
		log_TCP.print(2, "Type:     %d", type);
		log_TCP.print(4, "Length:   %d", length);
		log_TCP.print(4, "Num Inst: %d", num_instances);
	}

};
std::vector<message_basic> unrecognised_messages;
void add_unrecognised_msg(CNetworkMessage* msg)
{
	for (message_basic& m : unrecognised_messages)
	{
		if (m.type == msg->GetType())
		{
			++m;
			return;
		}
	}
	unrecognised_messages.push_back(msg);
}
void print_types()
{
	log_TCP.print("Unrecognised messages:");
	if (unrecognised_messages.empty())
	{
		log_TCP.print("none");
	}
	else
	for (message_basic& msg : unrecognised_messages)
	{
		msg.print();
	}
}

//// Receive Message
/// print
void print_bound(CNetworkMessage* msg)
{
	log_TCP.print(4, "Boundary line:");
	log_TCP.print(8, "iLevel: %d", msg->GetInt(8));
	log_TCP.print(8, "sX: %f", msg->GetFloatLittle(12));
	log_TCP.print(8, "sY: %f", msg->GetFloatLittle(16));
	log_TCP.print(8, "sZ: %f", msg->GetFloatLittle(20));
	log_TCP.print(8, "eX: %f", msg->GetFloatLittle(24));
	log_TCP.print(8, "ey: %f", msg->GetFloatLittle(28)); // this is not used
	log_TCP.print(8, "eZ: %f", msg->GetFloatLittle(32)); // this is Y
	log_TCP.print(8, "fHeight: %f", msg->GetFloatLittle(36));
}
void print_stair(CNetworkMessage* msg)
{
	// EXMS_SEND_TRANSIT<iNodeID><x><y><z><floor><fLength><fWidth><fAngle><fHeight><iLanes><iSteps>
	log_TCP.print(4, "Stair:");
	log_TCP.print(4, "length: %d", msg->Length());

	log_TCP.print(8, "iNodeID: %d", msg->GetInt(4));
	log_TCP.print(8, "x: %f", msg->GetFloatLittle(8));
	log_TCP.print(8, "y: %f", msg->GetFloatLittle(12));
	log_TCP.print(8, "z: %f", msg->GetFloatLittle(16));
	log_TCP.print(8, "floor: %d", msg->GetInt(20));
	log_TCP.print(8, "fLength: %f", msg->GetFloatLittle(24));
	log_TCP.print(8, "fWidth: %f", msg->GetFloatLittle(28));
	log_TCP.print(8, "fAngle: %f", msg->GetFloatLittle(32));
	log_TCP.print(8, "fHeight: %f", msg->GetFloatLittle(36));
	log_TCP.print(8, "iLanes: %d", msg->GetInt(40));
	log_TCP.print(8, "iSteps: %d", msg->GetInt(44));
}
void print_node(CNetworkMessage* msg)
{
	log_TCP.print(4, "Node:");
	log_TCP.print(8, "iNodeId: %d", msg->GetInt(4));
	log_TCP.print(8, "iNodeType: %d", msg->GetInt(8));
	log_TCP.print(8, "iObstacle: %d", msg->GetInt(12));
	log_TCP.print(8, "iLevel: %d", msg->GetInt(16));
	log_TCP.print(8, "sX: %f", msg->GetFloatLittle(20));
	log_TCP.print(8, "sY: %f", msg->GetFloatLittle(24)); // this is not used
	log_TCP.print(8, "sZ: %f", msg->GetFloatLittle(28)); // this is Y
	log_TCP.print(8, "floorHeight: %f", msg->GetFloatLittle(32));
}
void print_arc(CNetworkMessage* msg)
{
	// <iArc><iNode1><iNode2><Length><fObstacle><fLane1><iPos1><fLane2><iPos2>
	log_TCP.print(4, "Arc:");
	log_TCP.print(8, "iArcId: %d", msg->GetInt(4));
	log_TCP.print(8, "iNodeId1: %d", msg->GetInt(8));
	log_TCP.print(8, "iNodeId2: %d", msg->GetInt(12));
	log_TCP.print(8, "fLength: %f", msg->GetFloatLittle(16));
	log_TCP.print(8, "fObstacle: %f", msg->GetFloatLittle(20));
	log_TCP.print(8, "fLane1: %f", msg->GetFloatLittle(24));
	log_TCP.print(8, "iPos1: %d", msg->GetInt(28));
	log_TCP.print(8, "fLane2: %f", msg->GetFloatLittle(32));
	log_TCP.print(8, "iPos2: %d", msg->GetInt(36));
}
void print_object_inst(CNetworkMessage* msg)
{
	log_TCP.print(4, "ObjectInstance:");
	log_TCP.print(8, "iObjectId: %d", msg->GetInt(4));
	log_TCP.print(8, "iType: %d", msg->GetInt(8));
	log_TCP.print(8, "iLevel: %d", msg->GetInt(12));
	log_TCP.print(8, "fAngle: %f", msg->GetFloatLittle(16));
	log_TCP.print(8, "sX: %f", msg->GetFloatLittle(20));
	log_TCP.print(8, "sY: %f", msg->GetFloatLittle(24));
	log_TCP.print(8, "sZ: %f", msg->GetFloatLittle(28));
}
void print_person(CNetworkMessage* msg)
{
	log_TCP.print(4, "Person:");
	log_TCP.print(8, "iPersonId: %d", msg->GetInt(4));
	log_TCP.print(8, "aPET: %f", msg->GetFloatLittle(8));
	log_TCP.print(8, "NodeID: %d", msg->GetInt(12));
	log_TCP.print(8, "aDrive: %f", msg->GetFloatLittle(16));
	log_TCP.print(8, "anAgility: %f", msg->GetFloatLittle(20));
	log_TCP.print(8, "ObjectID: %d", msg->GetInt(24));
	log_TCP.print(8, "AttachmentPoint: %d", msg->GetInt(28));
}
void print_move_person(CNetworkMessage* msg)
{
	log_TCP.print(4, "Move person:");
	log_TCP.print(8, "iPersonId: %d", msg->GetInt(4));
	log_TCP.print(8, "aPET: %f", msg->GetFloatLittle(8));
	log_TCP.print(8, "NodeID: %d", msg->GetInt(12));
	log_TCP.print(8, "TSA: %f", msg->GetFloatLittle(16));
	log_TCP.print(8, "aDrive: %f", msg->GetFloatLittle(20));
	log_TCP.print(8, "anAgility: %f", msg->GetFloatLittle(24));
	log_TCP.print(8, "ObjectID: %d", msg->GetInt(28));
	log_TCP.print(8, "AttachmentPoint: %d", msg->GetInt(32));
}
void print_all(CNetworkMessage* msg)
{
	log_TCP.print(6, "Content:");
	for (int i = 0; i < msg->Length() - 3; i += 4)
	{
		log_TCP.print(8, "pos %d:", i);
		log_TCP.print(10, "intB:   %d", msg->GetInt(i));
		log_TCP.print(10, "floatL: %f", msg->GetFloatLittle(i));
	}
}
void print_default(CNetworkMessage* msg, int type)
{
	log_TCP.print(4, "Message with type %d not recognised!", type);
	//print_all(msg, false);
}
void print_test(CNetworkMessage* msg)
{
	log_TCP.print(6, "TEST MESSAGE:");
	log_TCP.print(8, "Type:   %d", msg->GetType());
	log_TCP.print(8, "Length: %d", msg->Length());
	for (int i = 0; i < msg->Length() - 3; i += 4)
	{
		log_TCP.print(8, "pos %d:", i);
		log_TCP.print(10, "intB:   %d", msg->GetInt(i));
		log_TCP.print(10, "floatL: %f", msg->GetFloatLittle(i));
	}
}
void print_to_file(CNetworkMessage* msg, bool sent)
{
	msg_str* message_str = get_type_code(msg->GetType());
	char buf[300];
	sprintf(buf, sent ? "sent " : "recv ");
	sprintf(buf + strlen(buf), "Time %.3f Seq %d Connx 0 :size %d code %d (%s) ", (((int)timeGetTime()) - tcp_con_time)*0.001f, sent ? msg_counter_sent : msg_counter_recv, msg->Length() + 4, msg->GetType(), message_str->s_code);
	
	int i = 0;
	for (; i < message_str->num_el; i++)
	{
		if (message_str->is_int[i])
		{
			sprintf(buf + strlen(buf), ", %d", msg->GetInt(i * 4 + 4));
		}
		else
		{
			sprintf(buf + strlen(buf), ", %f", msg->GetFloatLittle(i * 4 + 4));
		}
	}

	int p = 0;
	if (message_str->add_pattern) // when the length of msg is variable
	{
		for (; i * 4 + 4 < msg->Length(); i++)
		{
			if (message_str->rep_pattern[p])
			{
				sprintf(buf + strlen(buf), ", %d", msg->GetInt(i * 4 + 4));
			}
			else
			{
				sprintf(buf + strlen(buf), ", %f", msg->GetFloatLittle(i * 4 + 4));
			}

			p = (p + 1) % message_str->rep_pattern.size();
		}
	}

	if (FileIO::output_tcp_log("TCP_log.txt", first_print_to_file, buf))
	{
		first_print_to_file = false;
	}
	else
	{
		log_main.print("TCP data could not be saved to file");
	}
}
void print_error_to_file(const char* message)
{
	if (FileIO::output_tcp_log("TCP_log.txt", first_print_to_file, message))
	{
		first_print_to_file = false;
	}
	else
	{
		log_main.print("TCP data could not be saved to file");
	}
}

/// do
void add_bound(CNetworkMessage* msg)
{
	s_object_space.TCP_add_bound(boundary_line(msg->GetFloatLittle(12), msg->GetFloatLittle(20), msg->GetFloatLittle(24), msg->GetFloatLittle(32), msg->GetInt(8), s_object_space.get_next_bound_id()));
}
void add_stair(CNetworkMessage* msg)
{
	s_object_space.TCP_add_stair(stair(msg->GetInt(4), msg->GetFloatLittle(8), msg->GetFloatLittle(16), msg->GetFloatLittle(32), msg->GetInt(40), msg->GetInt(44)+1, msg->GetFloatLittle(24), msg->GetFloatLittle(28), msg->GetFloatLittle(36), msg->GetInt(20)), true);
}
void add_node(CNetworkMessage* msg)
{
	if (msg->GetInt(8) != EX_NODE_TRANSIT)
	{
		s_object_space.TCP_add_node(node(msg->GetInt(4), msg->GetFloatLittle(20), msg->GetFloatLittle(28), msg->GetInt(8) == EX_NODE_LANDING ? 0 : -1, -1, msg->GetInt(16)));
	}
}
void add_arc(CNetworkMessage* msg)
{
	// <iArc><iNode1><iNode2><Length><fObstacle><fLane1><iPos1><fLane2><iPos2>
	float lane1 = msg->GetFloatLittle(24);
	float lane2 = msg->GetFloatLittle(32);

	bool lane1_node = lane1 < 0.0f;

	if (lane1_node && lane2 < 0.0f)
		s_object_space.TCP_add_arc(arc(msg->GetInt(8), msg->GetInt(12), msg->GetInt(4)));
	else
	{
		int nodeid = lane1_node ? msg->GetInt(8) : msg->GetInt(12);
		int stair_id = lane1_node ? msg->GetInt(12) : msg->GetInt(8);
		int pos = lane1_node ? msg->GetInt(36) : msg->GetInt(28);
		float lane = lane1_node ? lane2 : lane1;

		auto ids = s_object_space.get_stairpt_from_id(stair_id)->get_node_id(pos, lane);

		if (ids.first == -1)
		{
			log_TCP.print("stair arc not added, stair node id(s) not found");
			return;
		}
		
		// first arc
		s_object_space.TCP_add_arc(arc(nodeid, ids.first, msg->GetInt(4)));
		
		if (ids.second == -1) // just one arc
		{
			return;
		}
		
		// second arc
		s_object_space.TCP_add_arc(arc(nodeid, ids.second, msg->GetInt(4)));
	}
}
void add_object_inst(CNetworkMessage* msg)
{
	s_object_space.TCP_add_object_EX(msg->GetInt(4), msg->GetInt(8), msg->GetInt(12), msg->GetFloatLittle(16), msg->GetFloatLittle(20), msg->GetFloatLittle(28));
}
void add_person(CNetworkMessage* msg)
{
	s_object_space.TCP_add_person(msg->GetInt(4), msg->GetInt(12), msg->GetFloatLittle(16), msg->GetFloatLittle(20), msg->GetInt(24), msg->GetInt(28), simulating);
}
void remove_person(CNetworkMessage* msg)
{
	log_TCP.print("remove_person");
	s_object_space.TCP_remove_person(msg->GetInt(4));
}
void move_person(CNetworkMessage* msg)
{
	s_object_space.TCP_move_person(msg->GetInt(4), msg->GetInt(12), msg->GetFloatLittle(16), msg->GetFloatLittle(20), msg->GetFloatLittle(24), msg->GetInt(28), msg->GetInt(32));
}
void add_target_to_obj(CNetworkMessage* msg)
{
	log_TCP.print("add_target_to_obj");
	// < iObjectId > <iNodeID><iLevel><fxpos><fypos><fzpos><fSimulationTime><PersonID>
	s_object_space.go_to_target(msg->GetInt(4),msg->GetInt(8));
	if (msg->GetInt(32) != -1)
	{
		s_object_space.pick_up_person(msg->GetInt(4), msg->GetInt(32));
	}
}
void attach_to_obj(CNetworkMessage* msg)
{
	// < iObjectId > <iPersonId><iLocation>
	s_object_space.TCP_add_person_to_object(msg->GetInt(4), msg->GetInt(12), msg->GetInt(8), simulating);
}
void release_from_obj(CNetworkMessage* msg)
{
	// < iObjectId > <iPersonId><iNodeID>
	log_TCP.print("release_from_obj");
	s_object_space.TCP_remove_person_from_object(msg->GetInt(4), msg->GetInt(8), msg->GetInt(12));
}
void remove_object(CNetworkMessage* msg)
{
	log_TCP.print("remove_object");
	//s_object_space.TCP_remove_object(msg->GetInt(4)); // removes object now
	s_object_space.remove_object_task(msg->GetInt(4)); // sets task to remove object
}
void add_obj_sim(CNetworkMessage* msg)
{
	log_TCP.print("add_obj_sim");
	// <iObjectId><iObjectType><iLevel><fxpos><fypos><fzpos><frotation><fspeed>
	s_object_space.TCP_add_object_EX(msg->GetInt(4), msg->GetInt(8), msg->GetInt(12), msg->GetFloatLittle(28), msg->GetFloatLittle(16), msg->GetFloatLittle(24));
}
void generate_freespace()
{
	if (s_object_space.cspace_ready)
	{
		if (!s_object_space.freespace_ready)
		{
			s_object_space.generate_freespace();
		}
		else
		{
			log_TCP.print("freespace not generated - a freespace already exists");
		}
	}
	else
	{
		log_TCP.print("freespace not generated - cspace not ready");
	}
}
void reset_simulation(CNetworkMessage* msg)
{
	log_TCP.print("simulation reset");
	s_object_space.retrieve_state();
}
void update_sim_time(CNetworkMessage* msg, float& sim_time, float& time_step)
{
	float new_time = msg->GetFloatLittle(4);
	time_step = new_time - sim_time;
	sim_time = new_time;
	if (!tcp_log_min)
	{
		log_TCP.print("sim time:  %f", sim_time);
		log_TCP.print("time step: %f", time_step);
	}
}

//// Send Message
/// useful funcs
void print_sent(CNetworkMessage* msg)
{
	log_TCP.print("Sent msg:");
	log_TCP.print(2,"type: %d", msg->GetType());
	log_TCP.print(2,"length: %d", msg->Length());
	for (int i = 0; i < msg->Length() - 3; i += 4)
	{
		log_TCP.print(4, "pos: %d", i);
		log_TCP.print(6, "intB: %d", msg->GetInt(i));
		log_TCP.print(6, "FloatL: %f", msg->GetFloatLittle(i));
	}
}
void print_content(const std::vector<msg_element>& content)
{
	log_TCP.print("message content:");
	for (const msg_element& e : content)
	{
		if (e.is_int)
		{
			log_TCP.print(4,"i: %d", (int)e);
		}
		else
		{
			log_TCP.print(4,"f: %f", (float)e);
		}
	}
}
CNetworkMessage* create_message(int type, const std::vector<msg_element>& content)
{
	CNetworkMessage* msg = new CNetworkMessage(type, 4*(content.size()+1));
	msg->Set(type, 0);
	for (int pos = 0; pos < content.size(); ++pos)
	{
		const msg_element& e = content[pos];
		if (e.is_int)
		{
			msg->Set((int)e, 4 * (pos + 1));
		}
		else
		{
			msg->SetLittle((float)e, 4 * (pos + 1));
		}
	}
	return msg;
	// EXODUS receives length and everything in offset between 0 and length - 3
	// EXODUS takes off(0) as type, then everything else is content
	// to send a message with n contents, create message with off(0) = type, length = 4n+4 and off(4), off(8), ..., off(4n) = content
}
CNetworkMessage* create_empty_message(int type)
{
	CNetworkMessage* msg = new CNetworkMessage(type, 8);
	msg->Set(type, 0);
	return msg;
}

void create_and_send_message(int type, const std::vector<msg_element>& content)
{
	CNetworkMessage* msg = create_message(type, content);

	//print_sent(msg);

	if (CNetworkServerHandler::SendMessage(client_id, msg))
	{
		msg_counter_sent++;
		if (Log_to_file)
			print_to_file(msg, true);
		else
		{
			if (!simulating || !tcp_log_min)
			{
				log_TCP.print("message sent: %d", msg_counter_sent);
				log_TCP.print(2, "code: %d", type);
			}
		}
	}
	else
	{
		log_TCP.print("MESSAGE FAILED TO SEND: %d", msg_counter_sent + 1);
		log_TCP.print(2, "code: %d", type);
	}
	
}
void create_and_send_empty_message(int type)
{
	CNetworkMessage* msg = create_empty_message(type);

	//print_sent(msg);

	if (CNetworkServerHandler::SendMessage(client_id, msg))
	{
		msg_counter_sent++;
		if (Log_to_file)
			print_to_file(msg, true);
		else
		{
			if (!simulating || !tcp_log_min)
			{
				log_TCP.print("message sent: %d", msg_counter_sent);
				log_TCP.print(2, "code: %d", type);
			}
		}
	}
	else
	{
		log_TCP.print("MESSAGE FAILED TO SEND: %d", msg_counter_sent + 1);
		log_TCP.print(2, "code: %d", type);
	}
}
void send_example() // example of how to send message
{
	log_TCP.print("send example msg");

	// create vector of msg_elements by pushing back ints or floats in the order of message
	std::vector<msg_element> content;
	float f1 = 0.0f;
	int i2 = 3;
	float f3 = 42.634f;
	int i4 = 87;
	int i5 = -1;
	content.push_back(f1);
	content.push_back(i2);
	content.push_back(f3);
	content.push_back(i4);
	content.push_back(i5);

	// call create_and_send_message with type code and content
	int type = 32;
	create_and_send_message(type, content);
}
/// do
void send_object_config(const data_for_TCP::object_config& oc)
{
	UINT code = oc.deleted ? EXMS_REMOVEOBJECT : EXMS_MOVEOBJECT;
	create_and_send_message(code, oc.get_content());
}
void send_occupied_node(const data_for_TCP::occupied_nodes& on)
{
	if (on.node_ids.empty()) return;

	create_and_send_message(EXMS_NODEOBJECT, on.get_content());
}
void send_pvo(const data_for_TCP::pvo& vo)
{
	if (vo.node_obj_cost.empty()) return;

	create_and_send_message(EXMS_VELOCITYOBSTACLE, vo.get_content());
}
void send_attached_people(const data_for_TCP::attach_people& ap)
{
	create_and_send_message(EXMS_ATTACHOBJECTPERSON, ap.get_content());
}
void send_object_control(int object_id, int controlled) // 1 under OC control, 0 underr EXODUS control
{
	create_and_send_message(EXMS_UNDEREXTERNALCONTROL, { object_id ,controlled });
}
void send_control_all_objects()
{
	for (int& i : s_object_space.TCP_get_all_object_ids())
	{
		send_object_control(i, 1);
	}
}
void send_done()
{
	create_and_send_empty_message(EXMS_END_OF_OBJECT_DATA);
}
void send_stair_node(const data_for_TCP::stair_node& sn)
{
	create_and_send_message(EXMS_STAIR_NODE, sn.get_content());
}
void init_sync_objects()
{
	// GEO
	std::vector<stair_node> st_nodes = s_object_space.main_sim_init_stair_node();
	for (stair_node& sn : st_nodes)
	{
		send_stair_node(sn);
	}
	
	// OBJ
	std::vector<object_config> configs = s_object_space.main_sim_init_1();
	for (object_config& oc : configs)
	{
		send_object_config(oc);
	}

	std::vector<occupied_nodes> occ_nodes = s_object_space.main_sim_init_2();
	for (occupied_nodes& on : occ_nodes)
	{
		send_occupied_node(on);
	}
}

//// Loops
/// simulation
bool EXODUS_run_loop(float& sim_time, float& time_step)
{
	unsigned int last_msg_time = 0;
	bool paused = false;

	while (true)
	{
		last_msg_time = 0;
		CClientMessageContainer* msg_cnt;
		try
		{
			msg_cnt = CNetworkServerHandler::GetNewMessage();
		}
		catch (const std::exception& e)
		{
			print_error_to_file(e.what());
			log_TCP.print("Failed to check for new messages");
			continue;
		}

		if (msg_cnt)
		{
			msg_counter_recv++;
			CNetworkMessage* msg = msg_cnt->GetMessage();
			if (Log_to_file) print_to_file(msg, false);
			switch (msg->GetType())
			{
			// people
			case EXMS_MOVE_HEPTAD:
				move_person(msg);
				break;
			case EXMS_DELETE:
				remove_person(msg);
				break;
			// tasks
			case EXMS_TARGETOBJECT:
				add_target_to_obj(msg);
				break;
			case EXMS_ATTACHOBJECTPERSON:
				attach_to_obj(msg);
				break;
			case EXMS_RELEASEOBJECTPERSON:
				release_from_obj(msg);
				break;
			// object
			case EXMS_REMOVEOBJECT:
				remove_object(msg);
				break;
			case EXMS_ADDOBJECT:
				add_obj_sim(msg);
				break;
			// simulation
			case EXMS_SIMULATION_COMPLETED:
				log_TCP.print("Simulation completed");
				return false;
			case EXMS_SIMULATION_PAUSED:
				paused = !paused;
				log_TCP.print(paused ? "simulation paused": "simulation resumed");
				break;
			case EXMS_SIMULATION_RESET:
				reset_simulation(msg);
				return false;
			case EXMS_TIME:
				update_sim_time(msg, sim_time, time_step);
				break;
			case EXMS_WAITING_FOR_OBJECT_DATA:
				return true;
			// meta
			case EXMS_DISCONNECTED:
				log_TCP.print("Connection to client terminated!");
				accept_con = false;
				return false;
			// unknown
			default:
				log_TCP.print("unrecognised message %d", msg->GetType());
				break;
			}
		}
		else if (!paused) // no message (for timeout)
		{
			if (last_msg_time == 0)
			{
				last_msg_time = timeGetTime();
			}
			else if (timeGetTime() - last_msg_time > TIME_OUT)
			{
				log_TCP.print("EXODUS run timed out");
				return false;
			}
		}
	}
}
void sim_loop(CNetworkMessage* start_msg)
{
	log_TCP.print("Simulation started");

	float sim_time = 0.0f;
	float time_step = 0.1666666f;
	int step = 0;
	bool first = true;
	simulating = true;

	while (EXODUS_run_loop(sim_time, time_step))
	{
		if (!tcp_log_min) log_TCP.print("OC_run_started");

		auto start_timer = timeGetTime();

		++step;
		float this_time_step = time_step / s_object_space.num_runs_per_sim;
		std::vector<object_config> configs;
		std::vector<occupied_nodes> occ_nodes;
		std::vector<pvo> pvos;

		// RUN SIM STEPS
		for (int r = 0; r < s_object_space.num_runs_per_sim; ++r)
		{
			//// STEP 1
			std::vector<object_config> this_configs = s_object_space.main_sim_step_1(this_time_step, first);

			for (object_config& t_oc : this_configs)
			{
				bool not_added = true;
				for (object_config& oc : configs)
				{
					if (oc.id == t_oc.id)
					{
						oc = t_oc;
						not_added = false;
						break;
					}
				}

				if (not_added)
					configs.push_back(t_oc);
			}

			//// STEP 2
			std::vector<occupied_nodes> this_occ_nodes = s_object_space.main_sim_step_2();

			for (occupied_nodes& t_on : this_occ_nodes)
			{
				bool not_added = true;
				for (occupied_nodes& on : occ_nodes)
				{
					if (on.object_id == t_on.object_id)
					{
						on = t_on;
						not_added = false;
						break;
					}
				}

				if (not_added)
					occ_nodes.push_back(t_on);
			}

			//// STEP 3
			std::vector<pvo> this_pvos = s_object_space.main_sim_step_3(first);

			for (pvo& t_vo : this_pvos)
			{
				bool not_added = true;
				for (pvo& vo : pvos)
				{
					if (t_vo.person_id == vo.person_id)
					{
						for (auto& t_noc : t_vo.node_obj_cost)
						{
							for (auto& t_oc : t_noc.second)
							{
								vo.add_node_obj_cost(t_noc.first, t_oc.first, t_oc.second);
							}
						}
						not_added = false;
						break;
					}
				}

				if (not_added)
					pvos.push_back(t_vo);
			}

			auto comp_time_ms = timeGetTime() - start_timer;
			//// OUTPUT DATA
			s_object_space.output_sim_data("data_sim_TCP.csv", step, r, comp_time_ms, sim_time, first);
		}

		//// SEND DATA
		for (object_config& oc : configs)
		{
			send_object_config(oc);
		}

		for (occupied_nodes& on : occ_nodes)
		{
			send_occupied_node(on);
		}

		for (pvo& vo : pvos)
		{
			send_pvo(vo);
		}

		//// SEND ADDITIONAL DATA
		for (attach_people& ap : s_object_space.attached_people_store)
		{
			send_attached_people(ap);
		}
		s_object_space.attached_people_store.clear();

		send_done();

		first = false;

		if (!tcp_log_min) log_TCP.print("OC_run_complete");
	}

	simulating = false;

	log_TCP.print("Simulation stopped");
}
/// main
void TCP_main_loop()
{
	Log_to_file = log_TCP.to_file;
	if (Log_to_file)
	{
		log_TCP.print("TCP log to file enabled");
	}

	log_TCP.print("TCP loop started");
	UINT PortID = s_object_space.tcp_PortID;
	
	std::string aPortIDStr = std::to_string(static_cast<long long>(PortID));
	std::vector<char> port_char_array(aPortIDStr.begin(), aPortIDStr.end());
	port_char_array.push_back(0);

	accept_con = CNetworkServerHandler::AcceptConnections(&port_char_array[0]);

	if (accept_con)
		log_TCP.print("Accepting connections from PortID %d", (int)PortID);

	tcp_con_time = timeGetTime();

	while (accept_con)
	{
		CClientMessageContainer* msg_cnt;
		try
		{
			msg_cnt = CNetworkServerHandler::GetNewMessage();
		}
		catch (const std::exception& e)
		{
			print_error_to_file(e.what());
			log_TCP.print("Failed to check for new messages");
			continue;
		}

		if (msg_cnt)
		{
			msg_counter_recv++;
			CNetworkMessage* msg = msg_cnt->GetMessage();
			if (Log_to_file) print_to_file(msg, false);
			switch (msg->GetType())
			{
			// Setup
			case EXMS_ADD_WALL:
				TCP_print_only ? print_bound(msg) : add_bound(msg);
				break;
			case EXMS_SEND_NODE:
				TCP_print_only ? print_node(msg) : add_node(msg);
				break;
			case  EXMS_SEND_TRANSIT:
				TCP_print_only ? print_stair(msg) : add_stair(msg);
				break;
			case EXMS_SEND_ARC:
				TCP_print_only ? print_arc(msg) : add_arc(msg);
				break;
			case EXMS_ADD_HEPTAD:
				TCP_print_only ? print_person(msg) : add_person(msg);
				break;
			case EXMS_SEND_OBJECT:
				// also generates cspace if not already generated
				TCP_print_only ? print_object_inst(msg) : add_object_inst(msg);
				break;
			case EXMS_EXEC_SFE_COMPLETE:
				generate_freespace();
				send_control_all_objects();
				init_sync_objects();
				s_object_space.save_state();
				log_TCP.print("OC ready for simulation!!");
				break;
			// Simulation
			case EXMS_SIMULATION_STARTED:
				sim_loop(msg);
				break;
			case EXMS_SIMULATION_RESET:
				reset_simulation(msg);
				break;
			// TCP control
			case EXMS_DISCONNECTED:
				log_TCP.print(4, "Connection to client terminated!");
				accept_con = false;
				break;
			// Not recognised
			default:
				add_unrecognised_msg(msg);
				print_default(msg, msg->GetType());
				break;
			}
		}
	}

	log_TCP.print("TCP loop ended");
}