#ifndef TRACE_MESSAGE_H
#define TRACE_MESSAGE_H

#include<string>
#include<argos2/common/utility/datatypes/datatypes.h>

using namespace argos;

enum EMessageType {
  // This message tells us that new control loop has started.
  // Mostly useful for debugging purposes.
  CONTROL_LOOP_START = 0,
  EXPLORE            = 1,
  RETURN_TO_NEST     = 2,
  REST               = 3,
  COLLISION          = 4,
  PICK_UP_ITEM       = 5,
  DROP_ITEM          = 6,
  // Some debugging messages
  MESSAGE_QUEUE_LENGTH = -1
};

// A single log message
//
class CTraceMessage {
 public:
  // The string that should be written to the trace output
  virtual std::string Format(UInt32 time) = 0; 
  // The Id of the robot, as string
  std::string GetRobotId();
  // Constructor and destructor
  CTraceMessage(UInt32 robotId);
  virtual ~CTraceMessage();

 protected:
  // Id of the robot doing the logging
  UInt32 m_unRobotId;              
};

class CExploreTrace : public CTraceMessage {
 public:
  CExploreTrace(UInt32 robotId);
  std::string Format(UInt32 time);
};

class CReturnTrace : public CTraceMessage {
 public:
  CReturnTrace(UInt32 robotId);
  std::string Format(UInt32 time);
};

class CRestTrace : public CTraceMessage {
 public:
  CRestTrace(UInt32 robotId);
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
