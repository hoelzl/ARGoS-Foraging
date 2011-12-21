#include "trace_message.h"
#include <string>
#include<sstream>

using namespace argos;

CTraceMessage::CTraceMessage(UInt32 robotId) :
  m_unRobotId(robotId) {}

CTraceMessage::~CTraceMessage() {
}

std::string CTraceMessage::GetRobotId() {
  std::stringstream result;
  result << m_unRobotId;
  return result.str();
}


CExploreTrace::CExploreTrace(UInt32 robotId) :
  CTraceMessage(robotId) {}

std::string CExploreTrace::Format(UInt32 time) {
  std::stringstream result;
  result << time << ","
	 << m_unRobotId << ","
	 << EXPLORE;
  return result.str();
}


CReturnTrace::CReturnTrace(UInt32 robotId) :
  CTraceMessage(robotId) {}

std::string CReturnTrace::Format(UInt32 time) {
  std::stringstream result;
  result << time << ","
	 << m_unRobotId << ","
	 << RETURN_TO_NEST;
  return result.str();
}



CRestTrace::CRestTrace(UInt32 robotId) :
  CTraceMessage(robotId) {}

std::string CRestTrace::Format(UInt32 time) {
  std::stringstream result;
  result << time << ","
	 << m_unRobotId << ","
	 << REST;
  return result.str();
}


CCollisionTrace::CCollisionTrace(UInt32 robotId) :
  CTraceMessage(robotId) {}

std::string CCollisionTrace::Format(UInt32 time) {
  std::stringstream result;
  result << time << ","
	 << m_unRobotId << ","
	 << COLLISION;
  return result.str();
}


CPickUpItemTrace::CPickUpItemTrace(UInt32 robotId) :
  CTraceMessage(robotId) {}

std::string CPickUpItemTrace::Format(UInt32 time) {
  std::stringstream result;
  result << time << ","
	 << m_unRobotId << ","
	 << PICK_UP_ITEM;
  return result.str();
}


CDropItemTrace::CDropItemTrace(UInt32 robotId) :
  CTraceMessage(robotId) {}

std::string CDropItemTrace::Format(UInt32 time) {
  std::stringstream result;
  result << time << ","
	 << m_unRobotId << ","
	 << DROP_ITEM;
  return result.str();
}

