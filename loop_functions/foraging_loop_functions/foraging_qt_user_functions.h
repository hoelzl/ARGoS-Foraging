#ifndef FORAGING_QT_USER_FUNCTIONS_H
#define FORAGING_QT_USER_FUNCTIONS_H

#include <argos2/simulator/visualizations/qt-opengl/qtopengl_user_functions.h>

using namespace argos;

class CForagingQTUserFunctions : public CQTOpenGLUserFunctions {

public:
   
   virtual void Draw(CFootBotEntity& c_entity);

private:

};

#endif
