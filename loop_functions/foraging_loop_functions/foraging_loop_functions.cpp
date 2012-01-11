#include "foraging_loop_functions.h"
#include <argos2/simulator/simulator.h>
#include <argos2/common/utility/configuration/argos_configuration.h>
#include <argos2/common/utility/datatypes/any.h>
#include <argos2/simulator/space/entities/footbot_entity.h>
#include <controllers/footbot_foraging/trace_message.h>

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

CForagingLoopFunctions::CForagingLoopFunctions() :
  ForagingArenaSideX(-0.9f, 1.7f),
  ForagingArenaSideY(-1.7f, 1.7f),
  Floor(NULL),
  RNG(NULL),
  AggregatedEvents(STATE_SIZE, 0) {
}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

void CForagingLoopFunctions::Init(TConfigurationNode& node) {
  try {
    TConfigurationNode& foraging = GetNode(node, "foraging");
    //  Get a pointer to the floor entity 
    Floor = &Space().GetFloorEntity();

    GetNodeAttribute(foraging, "items", InitialFoodItems);
    //  Get the number of food items we want to be scattered from XML 
    GetNodeAttribute(foraging, "radius", FoodSquareRadius);
    FoodSquareRadius *= FoodSquareRadius;

    //  Create a new RNG 
    RNG = CARGoSRandom::CreateRNG("argos");
    //  Distribute uniformly the items in the environment 
    for(UInt32 i = 0; i < InitialFoodItems; ++i) {
      FoodPos.push_back(CVector2(RNG->Uniform(ForagingArenaSideX),
				 RNG->Uniform(ForagingArenaSideY)));
    }
    //  Get the mean time for food drops from XML 
    GetNodeAttribute(foraging, "food_drop_mean", FoodDropMean);
    //  Initialize the next food drop 
    NextFoodDrop = RNG->Exponential(FoodDropMean);

    //  Get the trace output file name from XML 
    GetNodeAttribute(foraging, "trace_output", strTraceOutput);
    //  Open the file, erasing its contents 
    TraceOutput.open(strTraceOutput.c_str(), 
		     std::ios_base::trunc | std::ios_base::out);

    //  Get the collision output file name from XML 
    GetNodeAttribute(foraging, "collision_output", strCollisionOutput);
    //  Open the file, erasing its contents 
    CollisionOutput.open(strCollisionOutput.c_str(),
			 std::ios_base::trunc | std::ios_base::out);
    // Get the id for the robot for which we are logging collisions
    GetNodeAttribute(foraging, "id_for_collision_output", IdForCollisionOutput);

    //  Get the summary output file name from XML 
    GetNodeAttribute(foraging, "summary_output", strSummaryOutput);
    //  Open the file, erasing its contents 
    SummaryOutput.open(strSummaryOutput.c_str(),
		       std::ios_base::trunc | std::ios_base::out);
  }
  catch(CARGoSException& ex) {
    THROW_ARGOSEXCEPTION_NESTED("Error parsing loop functions!", ex);
  }
}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

void CForagingLoopFunctions::Reset() {
  //  Reset the next drop time 
  NextFoodDrop = RNG->Exponential(FoodDropMean);
  
  //  Zero the counters 
  InitializeSummaryData();

  //  Close the output streams 
  TraceOutput.close();
  CollisionOutput.close();
  SummaryOutput.close();
  //  Open the output streams, erasing their contents 
  TraceOutput.open(strTraceOutput.c_str(),
		   std::ios_base::trunc | std::ios_base::out);
  CollisionOutput.open(strCollisionOutput.c_str(),
		       std::ios_base::trunc | std::ios_base::out);
  SummaryOutput.open(strSummaryOutput.c_str(),
		     std::ios_base::trunc | std::ios_base::out);

  //  Distribute the items uniformly in the environment 
  FoodPos.clear();
  for(UInt32 i = 0; i < InitialFoodItems; ++i) {
    FoodPos.push_back(CVector2(RNG->Uniform(ForagingArenaSideX),
			       RNG->Uniform(ForagingArenaSideY)));
  }
}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

void CForagingLoopFunctions::Destroy() {
  //  Close the file 
  TraceOutput.close();
}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

CColor CForagingLoopFunctions::GetFloorColor(const CVector2& positionOnPlane) {
  if(positionOnPlane.GetX() < -1.0f) {
    return CColor::GRAY50;
  }
  for(UInt32 i = 0; i < FoodPos.size(); ++i) {
    if((positionOnPlane - FoodPos[i]).SquareLength() < FoodSquareRadius) {
      return CColor::BLACK;
    }
  }
  return CColor::WHITE;
}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

void CForagingLoopFunctions::WriteTraceMessages(CFootBotEntity *footBot,
						CFootBotForaging *controller) {
  std::vector<CTraceMessage*> *traceMessages = controller->GetTraceMessages();
  for (std::vector<CTraceMessage*>::iterator sit = traceMessages->begin();
       sit != traceMessages->end();
       ++sit) {
    // Trace messages were gathered in the previous loop...
    TraceOutput << (*sit)->Format(Space().GetSimulationClock() - 1)  << std::endl;
  }
}

void CForagingLoopFunctions::WriteCollisionMessages(CFootBotEntity *footBot,
						    CFootBotForaging *controller) {
  std::vector<CTraceMessage*> *collisionMessages = controller->GetCollisionMessages();
  if (controller->GetId() == IdForCollisionOutput) {
    for (std::vector<CTraceMessage*>::iterator sit = collisionMessages->begin();
	 sit != collisionMessages->end();
	 ++sit) {
      // Collision messages were gathered in the previous loop...
      CollisionOutput << (*sit)->Format(Space().GetSimulationClock() - 1)  << std::endl;
    }
  }
}

void CForagingLoopFunctions::ClearAllMessages(CFootBotForaging *controller) {
  controller->GetTraceMessages()->clear();
  controller->GetCollisionMessages()->clear();
}

void CForagingLoopFunctions::InitializeSummaryData() {
  for (UInt32 i = 0; i < AggregatedEvents.size(); ++i) {
    AggregatedEvents[i] = 0;
  }
}

void CForagingLoopFunctions::AddSummaryData(CFootBotEntity *footBot,
					    CFootBotForaging *controller) {
  // // Process all trace messages.
  // std::vector<CTraceMessage*> *traceMessages = controller->GetTraceMessages();
  // for (std::vector<CTraceMessage*>::iterator sit = traceMessages->begin();
  //      sit != traceMessages->end();
  //      ++sit) {
  //   ++AggregatedEvents[(*sit)->GetMessageType()];
  // }
  // // Process all collision messages.
  // std::vector<CTraceMessage*> *collisionMessages = controller->GetCollisionMessages();
  // if (controller->GetId() == IdForCollisionOutput) {
  //   for (std::vector<CTraceMessage*>::iterator sit = collisionMessages->begin();
  // 	 sit != collisionMessages->end();
  // 	 ++sit) {
  //     ++AggregatedEvents[(*sit)->GetMessageType()];
  //   }
  // }
  ++AggregatedEvents[controller->GetState()];
}

void CForagingLoopFunctions::WriteSummaryOutput() {
  SummaryOutput << Space().GetSimulationClock() - 1;
  for (UInt32 i = 0; i < AggregatedEvents.size(); ++i) {
    SummaryOutput << "," << AggregatedEvents[i];
  }
  SummaryOutput << "," << FoodPos.size();
  SummaryOutput << std::endl;
}

void CForagingLoopFunctions::FlushOutputStreams() {
  TraceOutput << std::flush;
  CollisionOutput << std::flush;
  SummaryOutput << std::flush;
}

void CForagingLoopFunctions::FindFoodItem(CFootBotForaging::SFoodData& foodData,
					  CVector2 pos) {
  for(size_t i = 0; i < FoodPos.size(); ++i) {
    // TODO: I've added a random epsilon (0.1), since the robots and
    // the loop function don't exactly agree when a robot is on top of
    // a food item.  Figure out why this is the case and fix it
    // properly. --tc
    if((pos - FoodPos[i]).SquareLength() <= FoodSquareRadius + 0.1) {
      // If so, we delete that item 
      std::vector<CVector2>::iterator it = FoodPos.begin();
      advance(it, i);
      FoodPos.erase(it);
      
      // The foot-bot is now carrying an item 
      foodData.HasFoodItem = true;
      // The floor texture must be updated 
      Floor->SetChanged();
      return;
    }
  }
  LOGERR << "Could not find food item!" << std::endl;
}

void CForagingLoopFunctions::PrePhysicsEngineStep() {
  //  Logic to pick and drop food items 
  /*
   * If the time has come to drop a new food item, do so
   * If a robot is in the nest, drop the food item
   * If a robot is on a food item, pick it
   * Each robot can carry only one food item per time
   */
  UInt32 currentClock = Space().GetSimulationClock();
  if (currentClock >= NextFoodDrop) {
    // Determine the time for the next food drop
    NextFoodDrop += RNG->Exponential(FoodDropMean);
    // Drop a new food item
    FoodPos.push_back(CVector2(RNG->Uniform(ForagingArenaSideX),
			       RNG->Uniform(ForagingArenaSideY)));
    // Update the floor texture
    Floor->SetChanged();

  }
  
  // Get a collection of all footbots
  CSpace::TAnyEntityMap& footbots = Space().GetEntitiesByType("footbot_entity");
  
  InitializeSummaryData();
  for(CSpace::TAnyEntityMap::iterator it = footbots.begin();
      it != footbots.end();
      ++it) {
    //  Get handle to foot-bot entity and controller 
    CFootBotEntity& footBot = *any_cast<CFootBotEntity*>(it->second);
    CFootBotForaging& controller = dynamic_cast<CFootBotForaging&>(footBot.GetControllableEntity().GetController());
    
    WriteTraceMessages(&footBot, &controller);
    WriteCollisionMessages(&footBot, &controller);
    AddSummaryData(&footBot, &controller);
    ClearAllMessages(&controller);
    
    //  Get the position of the foot-bot on the ground as a CVector2 
    CVector2 pos;
    pos.Set(footBot.GetEmbodiedEntity().GetPosition().GetX(),
	    footBot.GetEmbodiedEntity().GetPosition().GetY());
    //  Get food data 
    CFootBotForaging::SFoodData& foodData = controller.GetFoodData();
    //  The foot-bot has a food item 
    if(foodData.HasFoodItem) {
      //  Check whether the foot-bot is in the nest 
      // This is terribly wrong!  Dropping of the food item should be
      // performed by the robot! --tc
      if(controller.IsOverNest()) {
	//  Place a new food item on the ground 
	//  Drop the food item 
	foodData.HasFoodItem = false;
	++foodData.TotalFoodItems;
	
	//  The floor texture must be updated 
	Floor->SetChanged();
      }
    }
    else {
      if (controller.IsPickingUpFoodItem())
	FindFoodItem(foodData, pos);
    }
  }
  
  WriteSummaryOutput();
  FlushOutputStreams();
}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

REGISTER_LOOP_FUNCTIONS(CForagingLoopFunctions, "foraging_loop_functions")
