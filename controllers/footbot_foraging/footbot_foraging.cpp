/* Include the controller definition */
#include "footbot_foraging.h"
/* Function definitions for XML parsing */
#include <argos2/common/utility/configuration/argos_configuration.h>
/* 2D vector definition */
#include <argos2/common/utility/math/vector2.h>
/* Logging */
#include <argos2/common/utility/logging/argos_log.h>

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

CFootBotForaging::SFoodData::SFoodData() :
   HasFoodItem(false),
   TotalFoodItems(0) {}

void CFootBotForaging::SFoodData::Reset() {
   HasFoodItem = false;
   TotalFoodItems = 0;
}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

CFootBotForaging::SDiffusionParams::SDiffusionParams() :
   GoStraightAngleRange(CRadians(-1.0f), CRadians(1.0f)) {}

void CFootBotForaging::SDiffusionParams::Init(TConfigurationNode& t_node) {
   try {
      CRange<CDegrees> cGoStraightAngleRangeDegrees(CDegrees(-10.0f), CDegrees(10.0f));
      GetNodeAttribute(t_node, "go_straight_angle_range", cGoStraightAngleRangeDegrees);
      GoStraightAngleRange.Set(ToRadians(cGoStraightAngleRangeDegrees.GetMin()),
                               ToRadians(cGoStraightAngleRangeDegrees.GetMax()));
      GetNodeAttribute(t_node, "delta", Delta);
   }
   catch(CARGoSException& ex) {
      THROW_ARGOSEXCEPTION_NESTED("Error initializing controller diffusion parameters.", ex);
   }
}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

void CFootBotForaging::SWheelTurningParams::Init(TConfigurationNode& t_node) {
   try {
      CDegrees cAngle;
      GetNodeAttribute(t_node, "hard_turn_angle_threshold", cAngle);
      HardTurnOnAngleThreshold = ToRadians(cAngle);
      GetNodeAttribute(t_node, "soft_turn_angle_threshold", cAngle);
      SoftTurnOnAngleThreshold = ToRadians(cAngle);
      GetNodeAttribute(t_node, "no_turn_angle_threshold", cAngle);
      NoTurnAngleThreshold = ToRadians(cAngle);
      GetNodeAttribute(t_node, "max_speed", MaxSpeed);
   }
   catch(CARGoSException& ex) {
      THROW_ARGOSEXCEPTION_NESTED("Error initializing controller wheel turning parameters.", ex);
   }
}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

CFootBotForaging::SStateData::SStateData() :
   ProbRange(0.0f, 1.0f) {}

void CFootBotForaging::SStateData::Init(TConfigurationNode& t_node) {
   try {
      GetNodeAttribute(t_node, "rest_to_explore_mean", RestToExploreMean);
      GetNodeAttribute(t_node, "minimum_search_for_place_in_nest_time", MinimumSearchForPlaceInNestTime);

   }
   catch(CARGoSException& ex) {
      THROW_ARGOSEXCEPTION_NESTED("Error initializing controller state parameters.", ex);
   }
}

void CFootBotForaging::SStateData::Reset() {
   State = STATE_RESTING;
   InNest = true;
   WakeUpTime = 0;
   TimeRested = 0;
   TimeSearchingForPlaceInNest = 0;
}

void CFootBotForaging::SStateData::SetNewWakeUpTime(UInt32 newTime) {
  WakeUpTime = newTime;
  TimeRested = 0;
}

void CFootBotForaging::SStateData::SaveState() {
  PreviousState = State;
}

bool CFootBotForaging::SStateData::RestingPeriodIsOver() {
  return TimeRested >= WakeUpTime;
}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

CFootBotForaging::CFootBotForaging() :
   Wheels(NULL),
   LEDs(NULL),
   RABA(NULL),
   RABS(NULL),
   Proximity(NULL),
   Light(NULL),
   Ground(NULL),
   RNG(NULL) {}

UInt32 CFootBotForaging::s_unIdCounter = 0;

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

void CFootBotForaging::Init(TConfigurationNode& t_node) {
   try {
     /*
      * Set Id
      */
     Id = s_unIdCounter++;

      /*
       * Initialize sensors/actuators
       */
      Wheels    = dynamic_cast<CCI_FootBotWheelsActuator* >  (GetRobot().GetActuator("footbot_wheels"      ));
      LEDs      = dynamic_cast<CCI_FootBotLedsActuator* >    (GetRobot().GetActuator("footbot_leds"        ));
      RABA      = dynamic_cast<CCI_RangeAndBearingActuator*> (GetRobot().GetActuator("range_and_bearing"   ));
      RABS      = dynamic_cast<CCI_RangeAndBearingSensor*>   (GetRobot().GetSensor  ("range_and_bearing"   ));
      Proximity = dynamic_cast<CCI_FootBotProximitySensor*>  (GetRobot().GetSensor  ("footbot_proximity"   ));
      Light     = dynamic_cast<CCI_FootBotLightSensor*>      (GetRobot().GetSensor  ("footbot_light"       ));
      Ground    = dynamic_cast<CCI_FootBotMotorGroundSensor*>(GetRobot().GetSensor  ("footbot_motor_ground"));
      /*
       * Parse XML parameters
       */
      /* Diffusion algorithm */
      DiffusionParams.Init(GetNode(t_node, "diffusion"));
      /* Wheel turning */
      WheelTurningParams.Init(GetNode(t_node, "wheel_turning"));
      /* Controller state */
      StateData.Init(GetNode(t_node, "state"));
   }
   catch(CARGoSException& ex) {
      THROW_ARGOSEXCEPTION_NESTED("Error initializing the foot-bot foraging controller for robot \"" << GetRobot().GetRobotId() << "\"", ex);
   }
   /*
    * Initialize other stuff
    */
   /* Create a random number generator. We use the 'argos' category so that creation, reset, seeding and cleanup are managed by ARGoS. */
   RNG = CARGoSRandom::CreateRNG("argos");
   Reset();
}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

void CFootBotForaging::ControlStep() {
  UpdateState();
  switch(StateData.State) {
  case SStateData::STATE_RESTING: {
    Rest();
    break;
  }
  case SStateData::STATE_EXPLORING: {
    Explore();
    break;
  }
  case SStateData::STATE_RETURN_TO_NEST: {
    ReturnToNest();
    break;
  }
  default: {
    LOGERR << "We can't be here, there's a bug!" << std::endl;
  }
  }
}


////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

void CFootBotForaging::Reset() {
   /* Reset robot state */
   StateData.Reset();
   /* Reset food data */
   FoodData.Reset();
   /* Set LED color */
   LEDs->SetAllColors(CColor::RED);
   /* Clear up the last exploration result */
   LastExplorationResult = LAST_EXPLORATION_NONE;
   TRangeAndBearingReceivedPacket::TRangeAndBearingData tData;
   tData[0] = LAST_EXPLORATION_NONE;
   RABA->SetData(tData);
}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

void CFootBotForaging::UpdateState() {
   /* Reset state flags */
   StateData.InNest = false;
   /* Read stuff from the ground sensor */
   const CCI_FootBotMotorGroundSensor::TReadings& tGroundReads = Ground->GetReadings();
   /*
    * You can say whether you are in the nest by checking the ground sensor
    * placed close to the wheel motors. It returns a value between 0 and 1.
    * It is 1 when the robot is on a white area, it is 0 when the robot
    * is on a black area and it is around 0.5 when the robot is on a gray area. 
    * The foot-bot has 4 sensors like this, two in the front
    * (corresponding to readings 0 and 1) and two in the back (corresponding
    * to reading 2 and 3).  Here we want the back sensors (readings 2 and 3) to
    * tell us whether we are on gray: if so, the robot is completely in the nest,
    * otherwise it's outside.
    */
   if(tGroundReads[2].Value > 0.25f &&
      tGroundReads[2].Value < 0.75f &&
      tGroundReads[3].Value > 0.25f &&
      tGroundReads[3].Value < 0.75f) {
      StateData.InNest = true;
   }
}
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

CVector2 CFootBotForaging::CalculateVectorToLight() {
   /* Get readings from light sensor */
   const CCI_FootBotLightSensor::TReadings& tLightReads = Light->GetReadings();
   /* Sum them together */
   CVector2 cAccumulator;
   for(size_t i = 0; i < tLightReads.size(); ++i) {
      cAccumulator += CVector2(tLightReads[i].Value, tLightReads[i].Angle);
   }
   /* If the light was perceived, return the vector */
   if(cAccumulator.Length() > 0.0f) {
      return CVector2(1.0f, cAccumulator.Angle());
   }
   /* Otherwise, return zero */
   else {
      return CVector2();
   }
}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

CVector2 CFootBotForaging::DiffusionVector(bool& b_collision) {
  // Get readings from proximity sensor
  const CCI_FootBotProximitySensor::TReadings& tProxReads
    = Proximity->GetReadings();
  // Sum them together
  CVector2 cDiffusionVector;
  for(size_t i = 0; i < tProxReads.size(); ++i) {
    cDiffusionVector += CVector2(tProxReads[i].Value, tProxReads[i].Angle);
  }
  // If the angle of the vector is small enough and the closest
  // obstacle is far enough, ignore the vector and go straight,
  // otherwise return it.
  if(DiffusionParams.GoStraightAngleRange.WithinMinBoundIncludedMaxBoundIncluded(cDiffusionVector.Angle()) &&
     cDiffusionVector.Length() < DiffusionParams.Delta ) {
    b_collision = false;
    return CVector2::X;
  }
  else {
    b_collision = true;
    cDiffusionVector.Normalize();
    return -cDiffusionVector;
  }
}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

void CFootBotForaging::SetWheelSpeedsFromVector(const CVector2& c_heading) {
  // Get the heading angle
  CRadians cHeadingAngle = c_heading.Angle().SignedNormalize();
  // Get the length of the heading vector
  Real fHeadingLength = c_heading.Length();
  // Clamp the speed so that it's not greater than MaxSpeed
  Real fBaseAngularWheelSpeed = Min<Real>(fHeadingLength, WheelTurningParams.MaxSpeed);

  SWheelTurningParams::ETurningMechanism oldTurningMechanism = WheelTurningParams.TurningMechanism;
  // Turning state switching conditions
  if(Abs(cHeadingAngle) <= WheelTurningParams.NoTurnAngleThreshold) {
    // No Turn, heading angle very small
    WheelTurningParams.TurningMechanism = SWheelTurningParams::NO_TURN;
  }
  else if(Abs(cHeadingAngle) > WheelTurningParams.HardTurnOnAngleThreshold) {
    // Hard Turn, heading angle very large
    WheelTurningParams.TurningMechanism = SWheelTurningParams::HARD_TURN;
  }
  else if(WheelTurningParams.TurningMechanism == SWheelTurningParams::NO_TURN &&
	  Abs(cHeadingAngle) > WheelTurningParams.SoftTurnOnAngleThreshold) {
    // Soft Turn, heading angle in between the two cases */
    WheelTurningParams.TurningMechanism = SWheelTurningParams::SOFT_TURN;
  }
  if (WheelTurningParams.TurningMechanism != SWheelTurningParams::NO_TURN &&
      oldTurningMechanism != WheelTurningParams.TurningMechanism) {
    CollisionMessages.push_back(new CCollisionTrace(Id));
  }
  
  // Wheel speeds based on current turning state
  Real fSpeed1, fSpeed2;
  switch(WheelTurningParams.TurningMechanism) {
  case SWheelTurningParams::NO_TURN: {
    // Just go straight
    fSpeed1 = fBaseAngularWheelSpeed;
    fSpeed2 = fBaseAngularWheelSpeed;
    break;
  }

  case SWheelTurningParams::SOFT_TURN: {
    // Both wheels go straight, but one is faster than the other
    Real fSpeedFactor = (WheelTurningParams.HardTurnOnAngleThreshold - Abs(cHeadingAngle)) / WheelTurningParams.HardTurnOnAngleThreshold;
    fSpeed1 = fBaseAngularWheelSpeed - fBaseAngularWheelSpeed * (1.0 - fSpeedFactor);
    fSpeed2 = fBaseAngularWheelSpeed + fBaseAngularWheelSpeed * (1.0 - fSpeedFactor);
    break;
  }
    
  case SWheelTurningParams::HARD_TURN: {
    // Opposite wheel speeds
    fSpeed1 = -WheelTurningParams.MaxSpeed;
    fSpeed2 =  WheelTurningParams.MaxSpeed;
    break;
  }
  }

  // Apply the calculated speeds to the appropriate wheels
   Real fLeftWheelSpeed, fRightWheelSpeed;
   if(cHeadingAngle > CRadians::ZERO) {
     // Turn Left
     fLeftWheelSpeed  = fSpeed1;
     fRightWheelSpeed = fSpeed2;
   }
   else {
     // Turn Right
     fLeftWheelSpeed  = fSpeed2;
     fRightWheelSpeed = fSpeed1;
   }
   // Finally, set the wheel speeds
   Wheels->SetLinearVelocity(fLeftWheelSpeed, fRightWheelSpeed);
}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

void CFootBotForaging::StartResting() {
  if (StateData.State == SStateData::STATE_RESTING) {
    LOGERR << "Trying to start resting when already in STATE_RESTING"
	   << std::endl;
  }
  StateData.SaveState();
  StateData.State = SStateData::STATE_RESTING;
  LEDs->SetAllColors(CColor::RED);
  TraceMessages.push_back(new CRestTrace(Id));
  Wheels->SetLinearVelocity(0.0f, 0.0f);
  StateData.SetNewWakeUpTime(RNG->Exponential(StateData.RestToExploreMean));
}

void CFootBotForaging::StartExploring() {
  if (StateData.State == SStateData::STATE_EXPLORING) {
    LOGERR << "Trying to start exploring when already in STATE_EXPLORING"
	   << std::endl;
  }
  StateData.SaveState();
  StateData.State = SStateData::STATE_EXPLORING;
  LEDs->SetAllColors(CColor::GREEN);
  TraceMessages.push_back(new CExploreTrace(Id));
}

void CFootBotForaging::StartReturningToNest() {
  if (StateData.State == SStateData::STATE_RETURN_TO_NEST) {
    LOGERR << "Trying to return to nest when already in STATE_RETURN_TO_NEST"
	   << std::endl;
  }
  StateData.SaveState();
  StateData.State = SStateData::STATE_RETURN_TO_NEST;
  LEDs->SetAllColors(CColor::BLUE);
  TraceMessages.push_back(new CReturnTrace(Id));
}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

void CFootBotForaging::Rest() {
  if (StateData.RestingPeriodIsOver()) {
    StartExploring();
  }
  else {
    ++StateData.TimeRested;
  }
}

/****************************************/
/****************************************/

void CFootBotForaging::Explore() {
  // We return to the nest when we have picked up a food item.
  if (FoodData.HasFoodItem) {
    StartReturningToNest();
    return;
  }
  else {
    // Get the diffusion vector to perform obstacle avoidance
    bool bCollision;
    CVector2 cDiffusion = DiffusionVector(bCollision);
    // If we are in the nest, we combine antiphototaxis with obstacle
    // avoidance. Outside the nest, we just use the diffusion vector
    if(StateData.InNest) {
      // The vector returned by CalculateVectorToLight() points to the
      // light. Thus, the minus sign is because we want to go away
      // from the light.
      SetWheelSpeedsFromVector(
        WheelTurningParams.MaxSpeed * cDiffusion -
	WheelTurningParams.MaxSpeed * 0.25f * CalculateVectorToLight());
    }
    else {
      // Use the diffusion vector only
      SetWheelSpeedsFromVector(WheelTurningParams.MaxSpeed * cDiffusion);
    }
  }
}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

void CFootBotForaging::ReturnToNest() {
  // Are we in the nest?
  if(StateData.InNest) {
    // Have we looked for a place long enough?
    if(StateData.TimeSearchingForPlaceInNest > StateData.MinimumSearchForPlaceInNestTime) {
      // Yes, rest
      StartResting();
      return;
    }
    else {
      /* No, keep looking */
      ++StateData.TimeSearchingForPlaceInNest;
    }
  }
  else {
    /* Still outside the nest */
    StateData.TimeSearchingForPlaceInNest = 0;
  }
  /* Keep going */
  bool bCollision;
  SetWheelSpeedsFromVector(
    WheelTurningParams.MaxSpeed * DiffusionVector(bCollision) +
    WheelTurningParams.MaxSpeed * CalculateVectorToLight());
}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

// This statement notifies ARGoS of the existence of the
// controller. It binds the class passed as first argument to the
// string passed as second argument. The string is then usable in the
// XML configuration file to refer to this controller. When ARGoS
// reads that string in the XML file, it knows which controller class
// to instantiate. See also the XML configuration files for an example
// of how this is used.
//
REGISTER_CONTROLLER(CFootBotForaging, "footbot_foraging_controller")
