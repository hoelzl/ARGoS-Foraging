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
  
  Real m_fFoodSquareRadius;
  CRange<Real> m_cForagingArenaSideX, m_cForagingArenaSideY;
  std::vector<CVector2> m_cFoodPos;
  CFloorEntity* m_pcFloor;
  CARGoSRandom::CRNG* m_pcRNG;
  
  std::string m_strOutput;
  std::ofstream m_cOutput;
  
  // The time when the next food item should be dropped
  UInt32 m_unNextFoodDrop;
  // The mean value for the exponential distributions between food drops
  Real m_fFoodDropMean;
  
  UInt32 m_unCollectedFood;
  SInt64 m_nEnergy;
  UInt32 m_unEnergyPerFoodItem;
  UInt32 m_unEnergyPerWalkingRobot;
};

#endif
