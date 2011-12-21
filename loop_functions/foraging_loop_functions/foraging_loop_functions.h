#ifndef FORAGING_LOOP_FUNCTIONS_H
#define FORAGING_LOOP_FUNCTIONS_H

#include <argos2/simulator/dynamic_linking/loop_functions.h>

#include <argos2/simulator/space/entities/floor_entity.h>
#include <argos2/common/utility/math/range.h>
#include <argos2/common/utility/argos_random.h>

#include <controllers/footbot_foraging/footbot_foraging.h>


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
  
  // The trace output files and streams
  std::string strTraceOutput;
  std::ofstream TraceOutput;
  void WriteTraceMessages(CFootBotEntity *footBot, CFootBotForaging *controller);

  // The id of the robot for which we are tracing collisions,
  // and the collision output files and streams.
  UInt32 IdForCollisionOutput;
  std::string strCollisionOutput;
  std::ofstream CollisionOutput;
  void WriteCollisionMessages(CFootBotEntity *footBot, CFootBotForaging *controller);

  // The output file and stream for aggregated results
  std::string strSummaryOutput;
  std::ofstream SummaryOutput;
  void WriteSummaryMessages();

  void FlushOutputStreams();
  
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
