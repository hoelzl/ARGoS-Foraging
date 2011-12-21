#ifndef TRACE_MESSAGE_H
#define TRACE_MESSAGE_H

#include<string>
#include<argos2/common/utility/datatypes/datatypes.h>

using namespace argos;

enum EMessageType {
  EXPLORE              = 0,
  PICK_UP_ITEM         = 1,
  RETURN_TO_NEST       = 2,
  DROP_ITEM            = 3,
  SEARCH_RESTING_PLACE = 4,
  REST                 = 5,
  COLLISION            = 6,
  // The size for an array indexed by (non-debugging) message types.
  MESSAGE_TYPE_SIZE,
  // Some debugging messages
  CONTROL_LOOP_START = -1,
  MESSAGE_QUEUE_LENGTH = -2
};

// A single log message
//
class CTraceMessage {
 public:
  // What type of message is this?
  virtual EMessageType GetMessageType() = 0;
  // The string that should be written to the trace output
  virtual std::string Format(UInt32 time); 
  // The Id of the robot, as string
  std::string GetRobotId();
  // Constructor and destructor
  CTraceMessage(UInt32 robotId);
  virtual ~CTraceMessage();

 protected:
  // Id of the robot doing the logging
  UInt32 RobotId;
};

class CExploreTrace : public CTraceMessage {
 public:
  EMessageType GetMessageType();
  CExploreTrace(UInt32 robotId);
};

class CPickUpItemTrace : public CTraceMessage {
 public:
  EMessageType GetMessageType();
  CPickUpItemTrace(UInt32 robotId);
};

class CReturnTrace : public CTraceMessage {
 public:
  EMessageType GetMessageType();
  CReturnTrace(UInt32 robotId);
};

class CDropItemTrace : public CTraceMessage {
 public:
  EMessageType GetMessageType();
  CDropItemTrace(UInt32 robotId);
};

class CSearchRestingPlaceTrace : public CTraceMessage {
 public:
  EMessageType GetMessageType();
  CSearchRestingPlaceTrace(UInt32 robotId);
};

class CRestTrace : public CTraceMessage {
 public:
  EMessageType GetMessageType();
  CRestTrace(UInt32 robotId);
};

class CCollisionTrace : public CTraceMessage {
 public:
  EMessageType GetMessageType();
  CCollisionTrace(UInt32 robotId);
};

#endif /* TRACE_MESSAGE_H */
