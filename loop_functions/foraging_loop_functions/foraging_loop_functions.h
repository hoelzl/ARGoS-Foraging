#ifndef FORAGING_LOOP_FUNCTIONS_H
#define FORAGING_LOOP_FUNCTIONS_H

#include <argos2/simulator/dynamic_linking/loop_functions.h>

#include <argos2/simulator/space/entities/floor_entity.h>
#include <argos2/common/utility/math/range.h>
#include <argos2/common/utility/argos_random.h>

using namespace argos;

class CForagingLoopFunctions : public CLoopFunctions {

 public:

  CForagingLoopFunctions();
  virtual ~CForagingLoopFunctions() {}
  
  virtual void Init(TConfigurationNode& t_tree);
  virtual void Reset();
  virtual void Destroy();
  virtual CColor GetFloorColor(const CVector2& c_position_on_plane);
  virtual void PrePhysicsEngineStep();
  
 private:

  inline CSpace& Space() {
    return m_cSpace;
  }
  
  Real FoodSquareRadius;
  CRange<Real> ForagingArenaSideX, ForagingArenaSideY;
  std::vector<CVector2> FoodPos;
  CFloorEntity* Floor;
  CARGoSRandom::CRNG* RNG;
  
  std::string strOutput;
  std::ofstream Output;
  
  // The time when the next food item should be dropped
  UInt32 NextFoodDrop;
  // The mean value for the exponential distributions between food drops
  Real FoodDropMean;
  
  UInt32 CollectedFood;
  SInt64 Energy;
  UInt32 EnergyPerFoodItem;
  UInt32 EnergyPerWalkingRobot;
};

#endif
