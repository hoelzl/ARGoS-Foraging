#include "foraging_loop_functions.h"
#include <argos2/simulator/simulator.h>
#include <argos2/common/utility/configuration/argos_configuration.h>
#include <argos2/common/utility/datatypes/any.h>
#include <argos2/simulator/space/entities/footbot_entity.h>
#include <controllers/footbot_foraging/footbot_foraging.h>

/****************************************/
/****************************************/

CForagingLoopFunctions::CForagingLoopFunctions() :
  m_cForagingArenaSideX(-0.9f, 1.7f),
  m_cForagingArenaSideY(-1.7f, 1.7f),
  m_pcFloor(NULL),
  m_pcRNG(NULL),
  m_unNextFoodDrop(0),
  m_unCollectedFood(0),
  m_nEnergy(0),
  m_unEnergyPerFoodItem(1),
  m_unEnergyPerWalkingRobot(1) {
}

/****************************************/
/****************************************/

void CForagingLoopFunctions::Init(TConfigurationNode& t_node) {
  try {
    TConfigurationNode& tForaging = GetNode(t_node, "foraging");
    /* Get a pointer to the floor entity */
    m_pcFloor = &m_cSpace.GetFloorEntity();
    /* Get the number of food items we want to be scattered from XML */
    UInt32 unFoodItems;
    GetNodeAttribute(tForaging, "items", unFoodItems);
    /* Get the number of food items we want to be scattered from XML */
    GetNodeAttribute(tForaging, "radius", m_fFoodSquareRadius);
    m_fFoodSquareRadius *= m_fFoodSquareRadius;
    /* Create a new RNG */
    m_pcRNG = CARGoSRandom::CreateRNG("argos");
    /* Distribute uniformly the items in the environment */
    for(UInt32 i = 0; i < unFoodItems; ++i) {
      m_cFoodPos.push_back(
			   CVector2(m_pcRNG->Uniform(m_cForagingArenaSideX),
				    m_pcRNG->Uniform(m_cForagingArenaSideY)));
    }
    /* Get the mean time for food drops from XML */
    GetNodeAttribute(tForaging, "food_drop_mean", m_fFoodDropMean);
    /* Initialize the next food drop */
    m_unNextFoodDrop = m_pcRNG->Exponential(m_fFoodDropMean);
    /* Get the output file name from XML */
    GetNodeAttribute(tForaging, "output", m_strOutput);
    /* Open the file, erasing its contents */
    m_cOutput.open(m_strOutput.c_str(), std::ios_base::trunc | std::ios_base::out);
    // m_cOutput << "# clock\twalking\tresting\tcollected_food\tenergy" << std::endl;
    /* Get energy gain per item collected */
    GetNodeAttribute(tForaging, "energy_per_item", m_unEnergyPerFoodItem);
    /* Get energy loss per walking robot */
    GetNodeAttribute(tForaging, "energy_per_walking_robot", m_unEnergyPerWalkingRobot);
  }
  catch(CARGoSException& ex) {
    THROW_ARGOSEXCEPTION_NESTED("Error parsing loop functions!", ex);
  }
}

/****************************************/
/****************************************/

void CForagingLoopFunctions::Reset() {
  /* Reset the next drop time */
  m_unNextFoodDrop = m_pcRNG->Exponential(m_fFoodDropMean);
  
  /* Zero the counters */
  m_unCollectedFood = 0;
  m_nEnergy = 0;
  /* Close the file */
  m_cOutput.close();
  /* Open the file, erasing its contents */
  m_cOutput.open(m_strOutput.c_str(), std::ios_base::trunc | std::ios_base::out);
  // m_cOutput << "# clock\twalking\tresting\tcollected_food\tenergy" << std::endl;
  /* Distribute uniformly the items in the environment */
  for(UInt32 i = 0; i < m_cFoodPos.size(); ++i) {
    m_cFoodPos[i].Set(m_pcRNG->Uniform(m_cForagingArenaSideX),
		      m_pcRNG->Uniform(m_cForagingArenaSideY));
  }
}

/****************************************/
/****************************************/

void CForagingLoopFunctions::Destroy() {
  /* Close the file */
  m_cOutput.close();
}

/****************************************/
/****************************************/

CColor CForagingLoopFunctions::GetFloorColor(const CVector2& c_position_on_plane) {
  if(c_position_on_plane.GetX() < -1.0f) {
    return CColor::GRAY50;
  }
  for(UInt32 i = 0; i < m_cFoodPos.size(); ++i) {
    if((c_position_on_plane - m_cFoodPos[i]).SquareLength() < m_fFoodSquareRadius) {
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
  UInt32 unCurrentClock = m_cSpace.GetSimulationClock();
  if (unCurrentClock >= m_unNextFoodDrop) {
    // Determine the time for the next food drop
    m_unNextFoodDrop += m_pcRNG->Exponential(m_fFoodDropMean);
    // Drop a new food item
    m_cFoodPos.push_back(CVector2(m_pcRNG->Uniform(m_cForagingArenaSideX),
				  m_pcRNG->Uniform(m_cForagingArenaSideY)));
    // Update the floor texture
    m_pcFloor->SetChanged();

  }

  UInt32 unWalkingFBs = 0;
  UInt32 unRestingFBs = 0;
  /* Check whether a robot is on a food item */
  CSpace::TAnyEntityMap& m_cFootbots = m_cSpace.GetEntitiesByType("footbot_entity");
  
  for(CSpace::TAnyEntityMap::iterator it = m_cFootbots.begin();
      it != m_cFootbots.end();
       ++it) {
    /* Get handle to foot-bot entity and controller */
    CFootBotEntity& cFootBot = *any_cast<CFootBotEntity*>(it->second);
    CFootBotForaging& cController = dynamic_cast<CFootBotForaging&>(cFootBot.GetControllableEntity().GetController());
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
      if(cPos.GetX() < -1.0f) {
	/* Place a new food item on the ground */
	/* m_cFoodPos[sFoodData.FoodItemIdx].Set(m_pcRNG->Uniform(m_cForagingArenaSideX),
					      m_pcRNG->Uniform(m_cForagingArenaSideY));
	*/
	/* Drop the food item */
	sFoodData.HasFoodItem = false;
	++sFoodData.TotalFoodItems;
	/* Increase the energy and food count */
	m_nEnergy += m_unEnergyPerFoodItem;
	++m_unCollectedFood;
	/* The floor texture must be updated */
	m_pcFloor->SetChanged();
      }
    }
    else {
      /* The foot-bot has no food item */
      /* Check whether the foot-bot is out of the nest */
      if(cPos.GetX() > -1.0f) {
	/* Check whether the foot-bot is on a food item */
	bool bDone = false;
	for(size_t i = 0; i < m_cFoodPos.size() && !bDone; ++i) {
	  if((cPos - m_cFoodPos[i]).SquareLength() < m_fFoodSquareRadius) {
	    /* If so, we delete that item */
	    std::vector<CVector2>::iterator it = m_cFoodPos.begin();
	    advance(it, i);
	    m_cFoodPos.erase(it);
	    /* And add an entry to the log */
	    m_cOutput << "PICKUP: " << m_cSpace.GetSimulationClock() << std::endl;
	    /* The foot-bot is now carrying an item */
	    sFoodData.HasFoodItem = true;
	    /* The floor texture must be updated */
	    m_pcFloor->SetChanged();
	    /* We are done */
	    bDone = true;
	  }
	}
      }
    }
  }
  /* Update energy expediture due to walking robots */
  m_nEnergy -= unWalkingFBs * m_unEnergyPerWalkingRobot;
  /* Output stuff to file */
  /*
  m_cOutput << m_cSpace.GetSimulationClock() << "\t"
	    << unWalkingFBs << "\t"
	    << unRestingFBs << "\t"
	    << m_unCollectedFood << "\t"
	    << m_nEnergy << std::endl;
  */
}

/****************************************/
/****************************************/

REGISTER_LOOP_FUNCTIONS(CForagingLoopFunctions, "foraging_loop_functions")
