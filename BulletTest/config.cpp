#include "reindeer.h"

#ifdef USE_OPENGL
bool USE_LEGACY = false;
#endif

int SCREEN_HEIGHT = 600;
int SCREEN_WIDTH = 800;
int DEPTHTEX_HEIGHT = SCREEN_HEIGHT;
int DEPTHTEX_WIDTH = SCREEN_WIDTH;

float PHYSICS_WORLD_SCALE = 1;

unordered_map<string, btCollisionShape*> CollisionShapes;