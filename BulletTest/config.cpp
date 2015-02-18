#include "reindeer.h"

#ifdef USE_OPENGL
bool USE_LEGACY = false;
#endif

int SCREEN_WIDTH = 800;
int SCREEN_HEIGHT = 600;
int DEPTHTEX_WIDTH = 512;
int DEPTHTEX_HEIGHT = 512;

float PHYSICS_WORLD_SCALE = 1;

unordered_map<string, btCollisionShape*> CollisionShapes;
