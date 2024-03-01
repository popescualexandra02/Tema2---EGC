#include "object3D_tema.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"


Mesh* object3D_tema::CreateTankBase(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float length_leg,
    float height_leg,
    float distance,
    float height_body,
    glm::vec3 color,
    glm::vec3 color2,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {   //leg1
        VertexFormat(glm::vec3(0, 0, 0), glm::vec3(0, 1, 1), color), //0
        VertexFormat(glm::vec3(0,0, length_leg), glm::vec3(0, 1, 1), color), //1
        VertexFormat(glm::vec3(height_leg,0, length_leg), glm::vec3(0, 1, 1), color), //2
        VertexFormat(glm::vec3(height_leg,0, 0), glm::vec3(0, 1, 1), color), //3
        VertexFormat(glm::vec3(height_leg,height_leg, 0), glm::vec3(0, 1, 1), color), //4
        VertexFormat(glm::vec3(0,height_leg, 0), glm::vec3(0, 1, 1), color), //5
        VertexFormat(glm::vec3(0,height_leg,length_leg), glm::vec3(0, 1, 1), color), //6
        VertexFormat(glm::vec3(height_leg,height_leg,length_leg), glm::vec3(0, 1, 1), color), //7

        //leg2
        VertexFormat(glm::vec3(distance, 0, 0), glm::vec3(0, 1, 1), color), //8
        VertexFormat(glm::vec3(distance,0, length_leg), glm::vec3(0, 1, 1), color), //9
        VertexFormat(glm::vec3(height_leg + distance,0, length_leg), glm::vec3(0, 1, 1), color), //10
        VertexFormat(glm::vec3(height_leg + distance,0, 0), glm::vec3(0, 1, 1), color), //11
        VertexFormat(glm::vec3(height_leg + distance,height_leg, 0), glm::vec3(0, 1, 1), color), //12
        VertexFormat(glm::vec3(distance,height_leg, 0), glm::vec3(0, 1, 1), color), //13
        VertexFormat(glm::vec3(distance,height_leg,length_leg), glm::vec3(0, 1, 1), color), //14
        VertexFormat(glm::vec3(height_leg + distance,height_leg,length_leg), glm::vec3(0, 1, 1), color), //15

        //first_body
        VertexFormat(glm::vec3(0,height_leg,length_leg / 8), glm::vec3(0, 1, 1), color2), //16
        VertexFormat(glm::vec3(0,height_leg,7 * length_leg /8), glm::vec3(0, 1, 1), color2), //17
        VertexFormat(glm::vec3(height_leg + distance,height_leg,7*length_leg/8), glm::vec3(0, 1, 1), color2), //18
        VertexFormat(glm::vec3(height_leg + distance,height_leg, length_leg / 8), glm::vec3(0, 1, 1), color2), //19
        VertexFormat(glm::vec3(0,height_leg + height_body, 2* length_leg / 8), glm::vec3(0, 1, 1), color2), //20
        VertexFormat(glm::vec3(0,height_leg + height_body, 6 *length_leg / 8), glm::vec3(0, 1, 1), color2), //21
        VertexFormat(glm::vec3(height_leg + distance,height_leg + height_body,6 * length_leg / 8), glm::vec3(0, 1, 1), color2), //22
        VertexFormat(glm::vec3(height_leg + distance,height_leg + height_body, 2 * length_leg / 8), glm::vec3(0, 1, 1), color2), //23


    };

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = {
    //leg1
    0,5,3,
    3,5,4,

    1,6,2,
    2,6,7,

    1,0,2,
    2,0,3,

    6,5,7,
    7,5,4,

    2,7,4,
    4,3,2,
    
    1,6,5,
    1,5,0,

    //leg2
    8,13,11,
    11,13,12,

    9,14,10,
    10,14,15,

    9,8,10,
    10,8,11,

    14,13,15,
    15,13,12,

    10,15,12,
    12,11,10,

    9,14,13,
    9,13,8,

    //body
    18,22,23,
    23,19,18,

    17,21,20,
    20,16,17,

    17,16,18,
    18,16,19,

    22,21,20,
    20,23,22,

    18,17,21,
    21,22,18,

    19,16,20,
    20,23,19


    };

    if (!fill) {
        square->SetDrawMode(GL_LINE_LOOP);
    }

    square->InitFromData(vertices, indices);
    return square;
}

