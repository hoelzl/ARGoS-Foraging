#include "foraging_loop_functions.h"
#include "../../controllers/footbot_foraging/trace_message.h"
#include <argos2/simulator/simulator.h>
#include <argos2/common/utility/configuration/argos_configuration.h>
#include <argos2/common/utility/datatypes/any.h>
#include <argos2/simulator/space/entities/footbot_entity.h>
#include <controllers/footbot_foraging/footbot_foraging.h>

/****************************************/
/****************************************/

CForagingLoopFunctions::CForagingLoopFunctions() :
  ForagingArenaSideX(-0.9f, 1.7f),
  ForagingArenaSideY(-1.7f, 1.7f),
  Floor(NULL),
  RNG(NULL),
  NextFoodDrop(0),
  CollectedFood(0),
  Energy(0),
  EnergyPerFoodItem(1),
  EnergyPerWalkingRobot(1) {
}

/****************************************/
/****************************************/

void CForagingLoopFunctions::Init(TConfigurationNode& t_node) {
  try {
    TConfigurationNode& tForaging = GetNode(t_node, "foraging");
    /* Get a pointer to the floor entity */
    Floor = &Space().GetFloorEntity();
    /* Get the number of food items we want to be scattered from XML */
    UInt32 unFoodItems;
    GetNodeAttribute(tForaging, "items", unFoodItems);
    /* Get the number of food items we want to be scattered from XML */
    GetNodeAttribute(tForaging, "radius", FoodSquareRadius);
    FoodSquareRadius *= FoodSquareRadius;
    /* Create a new RNG */
    RNG = CARGoSRandom::CreateRNG("argos");
    /* Distribute uniformly the items in the environment */
    for(UInt32 i = 0; i < unFoodItems; ++i) {
      FoodPos.push_back(
			   CVector2(RNG->Uniform(ForagingArenaSideX),
				    RNG->Uniform(ForagingArenaSideY)));
    }
    /* Get the mean time for food drops from XML */
    GetNodeAttribute(tForaging, "food_drop_mean", FoodDropMean);
    /* Initialize the next food drop */
    NextFoodDrop = RNG->Exponential(FoodDropMean);
    /* Get the output file name from XML */
    GetNodeAttribute(tForaging, "output", strOutput);
    /* Open the file, erasing its contents */
    Output.open(strOutput.c_str(), std::ios_base::trunc | std::ios_base::out);
    // Output << "# clock\twalking\tresting\tcollected_food\tenergy" << std::endl;
    /* Get energy gain per item collected */
    GetNodeAttribute(tForaging, "energy_per_item", EnergyPerFoodItem);
    /* Get energy loss per walking robot */
    GetNodeAttribute(tForaging, "energy_per_walking_robot", EnergyPerWalkingRobot);
  }
  catch(CARGoSException& ex) {
    THROW_ARGOSEXCEPTION_NESTED("Error parsing loop functions!", ex);
  }
}

/****************************************/
/****************************************/

void CForagingLoopFunctions::Reset() {
  /* Reset the next drop time */
  NextFoodDrop = RNG->Exponential(FoodDropMean);
  
  /* Zero the counters */
  CollectedFood = 0;
  Energy = 0;
  /* Close the file */
  Output.close();
  /* Open the file, erasing its contents */
  Output.open(strOutput.c_str(), std::ios_base::trunc | std::ios_base::out);
  // Output << "# clock\twalking\tresting\tcollected_food\tenergy" << std::endl;
  /* Distribute uniformly the items in the environment */
  for(UInt32 i = 0; i < FoodPos.size(); ++i) {
    FoodPos[i].Set(RNG->Uniform(ForagingArenaSideX),
		      RNG->Uniform(ForagingArenaSideY));
  }
}

/****************************************/
/****************************************/

void CForagingLoopFunctions::Destroy() {
  /* Close the file */
  Output.close();
}

/****************************************/
/****************************************/

CColor CForagingLoopFunctions::GetFloorColor(const CVector2& c_position_on_plane) {
  if(c_position_on_plane.GetX() < -1.0f) {
    return CColor::GRAY50;
  }
  for(UInt32 i = 0; i < FoodPos.size(); ++i) {
    if((c_position_on_plane - FoodPos[i]).SquareLength() < FoodSquareRadius) {
      return CColor::BLACK;
    }
  }
  return CColor::WHITE;
}

/****************************************/
/****************************************/

void CForagingLoopFunctions::PrePhysicsEngineStep() {
  /* Logic to pick and drop food items */
  /*
   * If the time has come to drop a new food item, do so
   * If a robot is in the nest, drop the food item
   * If a robot is on a food item, pick it
   * Each robot can carry only one food item per time
   */
  UInt32 unCurrentClock = Space().GetSimulationClock();
  if (unCurrentClock >= NextFoodDrop) {
    // Determine the time for the next food drop
    NextFoodDrop += RNG->Exponential(FoodDropMean);
    // Drop a new food item
    FoodPos.push_back(CVector2(RNG->Uniform(ForagingArenaSideX),
				  RNG->Uniform(ForagingArenaSideY)));
    // Update the floor texture
    Floor->SetChanged();

  }

  UInt32 unWalkingFBs = 0;
  UInt32 unRestingFBs = 0;
  /* Check whether a robot is on a food item */
  CSpace::TAnyEntityMap& Footbots = Space().GetEntitiesByType("footbot_entity");
  
  for(CSpace::TAnyEntityMap::iterator it = Footbots.begin();
      it != Footbots.end();
       ++it) {
    /* Get handle to foot-bot entity and controller */
    CFootBotEntity& cFootBot = *any_cast<CFootBotEntity*>(it->second);
    CFootBotForaging& cController = dynamic_cast<CFootBotForaging&>(cFootBot.GetControllableEntity().GetController());

    /* Write stored trace messages of the foot-bot */
    std::vector<CTraceMessage*> cTraceMessages = cController.GetTraceMessages();
    for (std::vector<CTraceMessage*>::iterator sit = cTraceMessages.begin();
	 sit != cTraceMessages.end();
	 ++sit) {
      Output << (*sit)->Format(Space().GetSimulationClock()-1)  << std::endl;
    }
    cTraceMessages.clear();

    /* Count how many foot-bots are in which state */
    if(! cController.IsResting()) ++unWalkingFBs;
    else ++unRestingFBs;
    /* Get the position of the foot-bot on the ground as a CVector2 */
    CVector2 cPos;
    cPos.Set(cFootBot.GetEmbodiedEntity().GetPosition().GetX(),
	     cFootBot.GetEmbodiedEntity().GetPosition().GetY());
    /* Get food data */
    CFootBotForaging::SFoodData& sFoodData = cController.GetFoodData();
    /* The foot-bot has a food item */
    if(sFoodData.HasFoodItem) {
      /* Check whether the foot-bot is in the nest */
      // This is terribly wrong!  Dropping of the food item should be
      // performed by the robot! --tc
      if(cPos.GetX() < -1.0f) {
	/* Place a new food item on the ground */
	/* Drop the food item */
	sFoodData.HasFoodItem = false;
	++sFoodData.TotalFoodItems;

	/* Log that the food was dropped. */
	CDropItemTrace cDropTrace(cController.GetId());
	Output << cDropTrace.Format(Space().GetSimulationClock()) << std::endl;

	/* Increase the energy and food count */
	Energy += EnergyPerFoodItem;
	++CollectedFood;
	/* The floor texture must be updated */
	Floor->SetChanged();
      }
    }
    else {
      /* The foot-bot has no food item */
      /* Check whether the foot-bot is out of the nest */
      if(cPos.GetX() > -1.0f) {
	/* Check whether the foot-bot is on a food item */
	bool bDone = false;
	for(size_t i = 0; i < FoodPos.size() && !bDone; ++i) {
	  if((cPos - FoodPos[i]).SquareLength() < FoodSquareRadius) {
	    /* If so, we delete that item */
	    std::vector<CVector2>::iterator it = FoodPos.begin();
	    advance(it, i);
	    FoodPos.erase(it);
	    /* And add an entry to the log */
	    CPickUpItemTrace cPickUpTrace(cController.GetId());
	    Output << cPickUpTrace.Format(Space().GetSimulationClock()) << std::endl;

	    /* The foot-bot is now carrying an item */
	    sFoodData.HasFoodItem = true;
	    /* The floor texture must be updated */
	    Floor->SetChanged();
	    /* We are done */
	    bDone = true;
	  }
	}
      }
    }
  }
  /* Update energy expediture due to walking robots */
  Energy -= unWalkingFBs * EnergyPerWalkingRobot;
  /* Output stuff to file */
  /*
  Output << Space().GetSimulationClock() << "\t"
	    << unWalkingFBs << "\t"
	    << unRestingFBs << "\t"
	    << CollectedFood << "\t"
	    << Energy << std::endl;
  */
  /* Flush output */
  Output << std::endl;
}

/****************************************/
/****************************************/

REGISTER_LOOP_FUNCTIONS(CForagingLoopFunctions, "foraging_loop_functions")
