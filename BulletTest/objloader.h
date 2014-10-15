#include <iostream>
#include <vector>
#include "reindeer.h"

bool loadOBJ(
        const char * path, 
        std::vector<RDRVEC3> & out_vertices, 
        std::vector<RDRVEC2> & out_uvs,
        std::vector<RDRVEC3> & out_normals
);