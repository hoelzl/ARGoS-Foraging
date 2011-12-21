#include "trace_message.h"
#include <string>
#include<sstream>

using namespace argos;

CTraceMessage::CTraceMessage(UInt32 robotId) :
  RobotId(robotId) {}

CTraceMessage::~CTraceMessage() {
}

std::string CTraceMessage::GetRobotId() {
  std::stringstream result;
  result << RobotId;
  return result.str();
}


CExploreTrace::CExploreTrace(UInt32 robotId) :
  CTraceMessage(robotId) {}

std::string CExploreTrace::Format(UInt32 time) {
  std::stringstream result;
  result << time << ","
	 << RobotId << ","
	 << EXPLORE;
  return result.str();
}


CReturnTrace::CReturnTrace(UInt32 robotId) :
  CTraceMessage(robotId) {}

std::string CReturnTrace::Format(UInt32 time) {
  std::stringstream result;
  result << time << ","
	 << RobotId << ","
	 << RETURN_TO_NEST;
  return result.str();
}



CRestTrace::CRestTrace(UInt32 robotId) :
  CTraceMessage(robotId) {}

std::string CRestTrace::Format(UInt32 time) {
  std::stringstream result;
  result << time << ","
	 << RobotId << ","
	 << REST;
  return result.str();
}


CCollisionTrace::CCollisionTrace(UInt32 robotId) :
  CTraceMessage(robotId) {}

std::string CCollisionTrace::Format(UInt32 time) {
  std::stringstream result;
  result << time << ","
	 << RobotId << ","
	 << COLLISION;
  return result.str();
}


CPickUpItemTrace::CPickUpItemTrace(UInt32 robotId) :
  CTraceMessage(robotId) {}

std::string CPickUpItemTrace::Format(UInt32 time) {
  std::stringstream result;
  result << time << ","
	 << RobotId << ","
	 << PICK_UP_ITEM;
  return result.str();
}


CDropItemTrace::CDropItemTrace(UInt32 robotId) :
  CTraceMessage(robotId) {}

std::string CDropItemTrace::Format(UInt32 time) {
  std::stringstream result;
  result << time << ","
	 << RobotId << ","
	 << DROP_ITEM;
  return result.str();
}

