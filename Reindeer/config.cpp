#include "reindeer.h"

#ifdef USE_OPENGL
bool USE_LEGACY = false;
#endif

int SCREEN_WIDTH = 800;
int SCREEN_HEIGHT = 600;
int DEPTHTEX_WIDTH = 1024;
int DEPTHTEX_HEIGHT = 1024;
bool DEBUG_SHADOWMAP = false;

float PHYSICS_WORLD_SCALE = 1;

unordered_map<string, btCollisionShape*> CollisionShapes;
