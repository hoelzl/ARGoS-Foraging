#ifndef TRACE_MESSAGE_H
#define TRACE_MESSAGE_H

#include<string>
#include<argos2/common/utility/datatypes/datatypes.h>

using namespace argos;

enum EMessageType {
  EXPLORE      = 0,
  COLLISION    = 1,
  PICK_UP_ITEM = 2,
  DROP_ITEM    = 3
};

/*
 * This structure holds the data for a single log message
 */
class CTraceMessage {
 public:
  virtual std::string Format(UInt32 time) = 0; // The string that should be written to the trace output
  std::string GetRobotId();                    // The Id of the robot, as string
  CTraceMessage(UInt32 robotId);
  virtual ~CTraceMessage();

 protected:
  UInt32 m_unRobotId;                          // Id of the robot doing the logging
};

class CExploreTrace : public CTraceMessage {
 public:
  CExploreTrace(UInt32 robotId);
  std::string Format(UInt32 time);
};

class CCollisionTrace : public CTraceMessage {
 public:
  CCollisionTrace(UInt32 robotId);
  std::string Format(UInt32 time);
};

class CPickUpItemTrace : public CTraceMessage {
 public:
  CPickUpItemTrace(UInt32 robotId);
  std::string Format(UInt32 time);
};

class CDropItemTrace : public CTraceMessage {
 public:
  CDropItemTrace(UInt32 robotId);
  std::string Format(UInt32 time);
};

#endif /* TRACE_MESSAGE_H */
