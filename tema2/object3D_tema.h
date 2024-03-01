#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"


namespace object3D_tema
{

    
    Mesh* CreateTankBase(const std::string& name, glm::vec3 leftBottomCorner, float length_leg, float height_leg,float distance, float height_body, glm::vec3 color, glm::vec3 color2, bool fill = false);


}

