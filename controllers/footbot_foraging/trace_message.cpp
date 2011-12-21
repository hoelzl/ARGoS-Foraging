#include "trace_message.h"
#include <string>
#include<sstream>

using namespace argos;

CTraceMessage::CTraceMessage(UInt32 robotId) :
  RobotId(robotId) {}

CTraceMessage::~CTraceMessage() {
}

std::string CTraceMessage::Format(UInt32 time) {
  std::stringstream result;
  result << time << ","
	 << RobotId << ","
	 << GetMessageType();
  return result.str();
}

std::string CTraceMessage::GetRobotId() {
  std::stringstream result;
  result << RobotId;
  return result.str();
}


CExploreTrace::CExploreTrace(UInt32 robotId) :
  CTraceMessage(robotId) {}

EMessageType CExploreTrace::GetMessageType() {
  return EXPLORE;
}


CPickUpItemTrace::CPickUpItemTrace(UInt32 robotId) :
  CTraceMessage(robotId) {}

EMessageType CPickUpItemTrace::GetMessageType() {
  return PICK_UP_ITEM;
}


CReturnTrace::CReturnTrace(UInt32 robotId) :
  CTraceMessage(robotId) {}

EMessageType CReturnTrace::GetMessageType() {
  return RETURN_TO_NEST;
}


CDropItemTrace::CDropItemTrace(UInt32 robotId) :
  CTraceMessage(robotId) {}

EMessageType CDropItemTrace::GetMessageType() {
  return DROP_ITEM;
}


CSearchRestingPlaceTrace::CSearchRestingPlaceTrace(UInt32 robotId) :
  CTraceMessage(robotId) {}

EMessageType CSearchRestingPlaceTrace::GetMessageType() {
  return SEARCH_RESTING_PLACE;
}


CRestTrace::CRestTrace(UInt32 robotId) :
  CTraceMessage(robotId) {}

EMessageType CRestTrace::GetMessageType() {
  return REST;
}


CCollisionTrace::CCollisionTrace(UInt32 robotId) :
  CTraceMessage(robotId) {}

EMessageType CCollisionTrace::GetMessageType() {
  return COLLISION;
}

