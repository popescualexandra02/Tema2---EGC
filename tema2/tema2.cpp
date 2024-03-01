#include "lab_m1/tema2/tema2.h"

#include <vector>
#include <string>
#include <iostream>
#include "random"

using namespace std;
using namespace m1;

std::random_device random_dist;
std::mt19937 mt_random_dist(random_dist());
std::uniform_real_distribution<float> randDistance_cladire(-15, 15);

std::random_device random_dim_w;
std::mt19937 mt_random_dim_w(random_dim_w());
std::uniform_real_distribution<float> randDim_cladire_w(0, 2);

std::random_device random_dim_h;
std::mt19937 mt_random_dim_h(random_dim_h());
std::uniform_real_distribution<float> randDim_cladire_h(1, 2);

Tema2::Tema2()
{
}


Tema2::~Tema2()
{
}
float length_leg = 0.8f;
float heigth_leg = 0.2f;
float distance_legs = 0.35f;
float heigth_body = 0.25f;
glm::vec3 color_leg = glm::vec3(0.75f, 0.72f, 0.42f);
glm::vec3 color_body1 = glm::vec3(0.4f, 0.42f, 0.18f);
glm::vec3 color_body2 = glm::vec3(0.4f, 0.42f, 0.18f);
glm::vec3 color_body3 = glm::vec3(0.43f, 0.55f, 0.24f);
glm::vec3 color_cylinder = glm::vec3(0.97f, 0.95f, 0.95f);
glm::vec3 color_cladire = glm::vec3(0.25f, 0.29f, 0.35f);
typedef struct cladiri {
    float random_distance_x;
    float random_distance_z;
    float height;

}cladiri;
cladiri cladire[60];
glm::vec3 corner = glm::vec3(-0.325f, 0, 0);

void Tema2::Generate_random_cladire(float x, float y, float z) {
    glm::mat4 modelMatrix = glm::mat4(1);

    modelMatrix = glm::translate(modelMatrix, glm::vec3(x, 0, z));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(1, y, 1));
    RenderSimpleMesh(meshes["cladire"], shaders["TemaShader"], modelMatrix);

}

void Tema2::CreateCircleMesh(float radius, float height, int segments, const glm::vec3& color)
{
    vector<VertexFormat> vertices;
    vector<unsigned int> indices;
    for (int i = 0; i <= segments; ++i) {
        float angleInDegrees = (360.0f / segments) * i;
        float angleInRadians = (angleInDegrees * M_PI) / 180.0f;
        float x = radius * std::cos(angleInRadians);
        float y = radius * std::sin(angleInRadians);

        // Bottom vertex
        vertices.push_back(VertexFormat(corner + glm::vec3(x, y, -height / 2.0f), color, glm::vec3(0, -1, 0)));

        // Top vertex
        vertices.push_back(VertexFormat(corner + glm::vec3(x, y, height / 2.0f), color, glm::vec3(0, 1, 0)));
    }

    // Create indices for the sides of the cylinder
    for (unsigned int i = 0; i < vertices.size() - 2; i += 2) {
        indices.push_back(i);
        indices.push_back(i + 1);
        indices.push_back(i + 2);

        indices.push_back(i + 2);
        indices.push_back(i + 1);
        indices.push_back(i + 3);
    }

    // Create indices for the bottom circle
    for (unsigned int i = 0; i < segments; ++i) {
        indices.push_back(0);
        indices.push_back(i * 2);
        indices.push_back((i + 1) * 2);
    }

    // Create indices for the top circle
    for (unsigned int i = 0; i < segments; ++i) {
        indices.push_back(1);
        indices.push_back((i + 1) * 2 + 1);
        indices.push_back(i * 2 + 1);
    }



    CreateMesh("tank_cilindru", vertices, indices);
}
void Tema2::CreateSphereMesh(float radius, int stacks, int sectors, const glm::vec3& color)
{
    vector<VertexFormat> vertices;
    vector<unsigned int> indices;

    for (int i = 0; i <= stacks; ++i) {
        float phi = M_PI * static_cast<float>(i) / stacks;
        float sinPhi = sin(phi);
        float cosPhi = cos(phi);

        for (int j = 0; j <= sectors; ++j) {
            float theta = 2.0f * M_PI * static_cast<float>(j) / sectors;
            float sinTheta = sin(theta);
            float cosTheta = cos(theta);

            float x = radius * sinPhi * cosTheta;
            float y = radius * sinPhi * sinTheta;
            float z = radius * cosPhi;

            vertices.push_back(VertexFormat(glm::vec3(x, y, z), color, glm::normalize(glm::vec3(x, y, z))));
        }
    }

    for (int i = 0; i < stacks; ++i) {
        for (int j = 0; j < sectors; ++j) {
            int first = i * (sectors + 1) + j;
            int second = first + sectors + 1;

            indices.push_back(first);
            indices.push_back(second);
            indices.push_back(first + 1);

            indices.push_back(second);
            indices.push_back(second + 1);
            indices.push_back(first + 1);
        }
    }

    CreateMesh("sphere", vertices, indices);
}


void Tema2::EndGAME() {
    glfwTerminate();
    exit(EXIT_SUCCESS);
}


double zNear = 0.1f;
double zFar = 200;
double top;
double bottom;
void Tema2::Init()
{
    renderCameraTarget = false;

    camera_tema = new implemented::Camera_tema();
    camera_tema->Set(glm::vec3(0, 1.5f, 2.5f), glm::vec3(0, 1, 0), glm::vec3(0, 0.1f, 0));
    

    //random_cladiri
    for (int i = 0; i < 25; i++) {
        cladire[i].random_distance_x = randDistance_cladire(mt_random_dist);
        cladire[i].random_distance_z = randDistance_cladire(mt_random_dist);
        cladire[i].height = randDim_cladire_h(mt_random_dim_h);

    }
    
    // Create tank_body
    {
        vector<VertexFormat> vertices
        {
            //leg1
        VertexFormat(corner, color_leg, glm::vec3(0,0,0)), //0
        VertexFormat(corner + glm::vec3(0,0, length_leg), color_leg, glm::vec3(0,0,0)), //1
        VertexFormat(corner + glm::vec3(heigth_leg,0, length_leg), color_leg, glm::vec3(0,0,0)), //2
        VertexFormat(corner + glm::vec3(heigth_leg,0, 0), color_leg, glm::vec3(0,0,0)), //3
        VertexFormat(corner + glm::vec3(heigth_leg,heigth_leg, 0), color_leg, glm::vec3(0,0,0)), //4
        VertexFormat(corner + glm::vec3(0,heigth_leg, 0), color_leg, glm::vec3(0,0,0)), //5
        VertexFormat(corner + glm::vec3(0,heigth_leg,length_leg), color_leg, glm::vec3(0,0,0)), //6
        VertexFormat(corner + glm::vec3(heigth_leg,heigth_leg,length_leg), color_leg, glm::vec3(0,0,0)), //7

        //leg2
        VertexFormat(corner + glm::vec3(distance_legs, 0, 0), color_leg, glm::vec3(0,0,0)), //8
        VertexFormat(corner + glm::vec3(distance_legs,0, length_leg), color_leg, glm::vec3(0,0,0)), //9
        VertexFormat(corner + glm::vec3(heigth_leg + distance_legs,0, length_leg), color_leg, glm::vec3(0,0,0)), //10
        VertexFormat(corner + glm::vec3(heigth_leg + distance_legs,0, 0), color_leg, glm::vec3(0,0,0)), //11
        VertexFormat(corner + glm::vec3(heigth_leg + distance_legs,heigth_leg, 0), color_leg, glm::vec3(0,0,0)), //12
        VertexFormat(corner + glm::vec3(distance_legs,heigth_leg, 0), color_leg, glm::vec3(0,0,0)), //13
        VertexFormat(corner + glm::vec3(distance_legs,heigth_leg,length_leg),color_leg, glm::vec3(0,0,0)), //14
        VertexFormat(corner + glm::vec3(heigth_leg + distance_legs,heigth_leg,length_leg), color_leg, glm::vec3(0,0,0)), //15

        //first_body
        VertexFormat(corner + glm::vec3(0,heigth_leg,length_leg / 8), color_body1, glm::vec3(0,0,0)), //16
        VertexFormat(corner + glm::vec3(0,heigth_leg,7 * length_leg / 8), color_body1, glm::vec3(0,0,0)), //17
        VertexFormat(corner + glm::vec3(heigth_leg + distance_legs,heigth_leg,7 * length_leg / 8), color_body1, glm::vec3(0,0,0)), //18
        VertexFormat(corner + glm::vec3(heigth_leg + distance_legs,heigth_leg, length_leg / 8), color_body1, glm::vec3(0,0,0)), //19
        VertexFormat(corner + glm::vec3(0,heigth_leg + heigth_body, 2 * length_leg / 8), color_body1, glm::vec3(0,0,0)), //20
        VertexFormat(corner + glm::vec3(0,heigth_leg + heigth_body, 6 * length_leg / 8), color_body1, glm::vec3(0,0,0)), //21
        VertexFormat(corner + glm::vec3(heigth_leg + distance_legs,heigth_leg + heigth_body,6 * length_leg / 8), color_body1, glm::vec3(0,0,0)), //22
        VertexFormat(corner + glm::vec3(heigth_leg + distance_legs,heigth_leg + heigth_body, 2 * length_leg / 8), color_body1, glm::vec3(0,0,0)), //23

        };

        vector<unsigned int> indices =
        {
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

        CreateMesh("tank_body_1", vertices, indices);
    }

    // Create tank_body_2
    {
        vector<VertexFormat> vertices
        {
            VertexFormat(corner + glm::vec3(0,heigth_leg + heigth_body,2.5f * length_leg / 8), color_body3, glm::vec3(0,0,1)),//0
            VertexFormat(corner + glm::vec3(0,heigth_leg + heigth_body,5.5f * length_leg / 8), color_body3, glm::vec3(0,0,1)),//1
            VertexFormat(corner + glm::vec3(heigth_leg + distance_legs,heigth_leg + heigth_body,5.5f * length_leg / 8), color_body3, glm::vec3(0,0,1)),//2
            VertexFormat(corner + glm::vec3(heigth_leg + distance_legs,heigth_leg + heigth_body,2.5f * length_leg / 8), color_body3, glm::vec3(0,0,1)),//3
            VertexFormat(corner + glm::vec3(0,heigth_leg + 1.5f * heigth_body,3 * length_leg / 8), color_body3, glm::vec3(0,0,1)),//4
            VertexFormat(corner + glm::vec3(0,heigth_leg + 1.5f * heigth_body,5 * length_leg / 8), color_body3, glm::vec3(0,0,1)),//5
            VertexFormat(corner + glm::vec3(heigth_leg + distance_legs,heigth_leg + 1.5f * heigth_body,5 * length_leg / 8), color_body3, glm::vec3(0,0,1)),//6
            VertexFormat(corner + glm::vec3(heigth_leg + distance_legs,heigth_leg + 1.5f * heigth_body,3 * length_leg / 8), color_body3, glm::vec3(0,0,1)),//7


        };

        vector<unsigned int> indices =
        {
            1,0,3,
            1,3,2,

            1,0,4,
            1,4,5,

            1,5,6,
            1,6,2,

            2,6,7,
            2,7,3,

            0,4,7,
            0,7,3,

            5,4,7,
            5,7,6
        };

        CreateMesh("tank_body_2", vertices, indices);
    }

    // Create cilindru
    {
        CreateCircleMesh(0.05f, 0.5f, 30, color_cylinder);
    }


    // Create plane
    {
        vector<VertexFormat> vertices
        {
            VertexFormat(glm::vec3(-1,0,-1), glm::vec3(0.64f, 0.8f, 0.35f), glm::vec3(0,0,0)),//0
            VertexFormat(glm::vec3(1,0,-1), glm::vec3(0.64f, 0.8f, 0.35f), glm::vec3(0,0,0)),//1
            VertexFormat(glm::vec3(1,0,1), glm::vec3(0.64f, 0.8f, 0.35f), glm::vec3(0,0,0)),//2
            VertexFormat(glm::vec3(-1,0,1), glm::vec3(0.64f, 0.8f, 0.35f), glm::vec3(0,0,0)),//3
        };

        vector<unsigned int> indices =
        {
            3, 0, 1,
            1, 2, 3
        };

        CreateMesh("plane", vertices, indices);
    }

    // Create cladire
    {
        vector<VertexFormat> vertices
        {
            VertexFormat(glm::vec3(0,0,0), color_cladire, glm::vec3(0,0,1)),//0
            VertexFormat(glm::vec3(1,0,0), color_cladire, glm::vec3(0,0,1)),//1
            VertexFormat(glm::vec3(1,0,1), color_cladire, glm::vec3(0,0,1)),//2
            VertexFormat(glm::vec3(0,0,1), color_cladire, glm::vec3(0,0,1)),//3
            VertexFormat(glm::vec3(0,1,1), color_cladire, glm::vec3(0,0,1)),//4
            VertexFormat(glm::vec3(0,1,0), color_cladire, glm::vec3(0,0,1)),//5
            VertexFormat(glm::vec3(1,1,0), color_cladire, glm::vec3(0,0,1)),//6
            VertexFormat(glm::vec3(1,1,1), color_cladire, glm::vec3(0,0,1)),//7

        };

        vector<unsigned int> indices =
        {
            0,1,2,
            0,2,3,

            4,5,7,
            7,5,6,

            2,7,6,
            6,1,2,

            3,4,5,
            5,0,3,

            2,3,4,
            2,4,7,

            0,5,1,
            5,6,1


        };

        CreateMesh("cladire", vertices, indices);
    }

    // Create sphere
    CreateSphereMesh(1.0f, 20, 20, glm::vec3(0,0,0));

    // Create a shader program for drawing face polygon with the color of the normal
    {
        Shader* shader = new Shader("TemaShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }
}

Mesh* Tema2::CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned int>& indices)
{
    unsigned int VAO = 0;
    // Create the VAO and bind it
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Create the VBO and bind it
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Send vertices data into the VBO buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

    // Create the IBO and bind it
    unsigned int IBO;
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

    // Send indices data into the IBO buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

    // Set vertex position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), 0);

    // Set vertex normal attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(sizeof(glm::vec3)));

    // Set texture coordinate attribute
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3)));

    // Set vertex color attribute
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3) + sizeof(glm::vec2)));
    // ========================================================================

    // Unbind the VAO
    glBindVertexArray(0);

    // Check for OpenGL errors
    CheckOpenGLError();

    // Mesh information is saved into a Mesh object
    meshes[name] = new Mesh(name);
    meshes[name]->InitFromBuffer(VAO, static_cast<unsigned int>(indices.size()));
    meshes[name]->vertices = vertices;
    meshes[name]->indices = indices;
    return meshes[name];
}


void Tema2::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0.62f, 0.85f, 0.94f, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}

float move_tank = 0;
float time_move_tank = 0;

float move_tank_rot = 0;
float end_game = 0;
int shoot = 0;
float shoot_gone = 0;
float shoot_move = -0.25f;
int fata = 0;
float muta_in_fata_x = 0;
float muta_in_fata_z = 0;
float rotate_tank = 0;
float inamic_x = 1;
float inamic_z = 1;
void Tema2::Update(float deltaTimeSeconds)
{
    //render tank
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, camera_tema->GetTargetPosition() - glm ::vec3(0,1,0));
        modelMatrix = glm::rotate(modelMatrix, rotate_tank, glm::vec3(0,1,0));
        RenderSimpleMesh(meshes["tank_body_1"], shaders["TemaShader"], modelMatrix);
    }
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, camera_tema->GetTargetPosition() - glm::vec3(0, 1, 0));
        modelMatrix = glm::rotate(modelMatrix, rotate_tank, glm::vec3(0, 1, 0));
        RenderSimpleMesh(meshes["tank_body_2"], shaders["TemaShader"], modelMatrix);
    }
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, camera_tema->GetTargetPosition() - glm::vec3(0, 1, 0));
        modelMatrix = glm::rotate(modelMatrix, rotate_tank, glm::vec3(0, 1, 0));
        modelMatrix = glm::translate(modelMatrix, glm::vec3(heigth_leg / 2 + distance_legs / 2, heigth_leg + 1.25 * heigth_body, 0.05f));
        RenderSimpleMesh(meshes["tank_cilindru"], shaders["TemaShader"], modelMatrix);
    }
    
    ////shoot_sphera
    ////printf("%d\n", shoot);
    /*shoot_gone += deltaTimeSeconds;
    if(shoot == 1)
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, camera_tema->GetTargetPosition());
        modelMatrix = glm::translate(modelMatrix, glm::vec3(heigth_leg / 2 + distance_legs / 2, heigth_leg + 1.25 * heigth_body, shoot_move));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.05f));
        RenderSimpleMesh(meshes["sphere"], shaders["TemaShader"], modelMatrix);
        shoot_move -= 0.1f;
        if (shoot_gone >= 2) {
            shoot -= 1;
            shoot_move = -0.25f;
            shoot_gone = 0;
        }
    }*/
    
    //END GAME
    time_move_tank += deltaTimeSeconds;
    end_game += deltaTimeSeconds;
    if (end_game >= 60) {
        EndGAME();
    }

    //INAMIC
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(inamic_x, 0, inamic_z));
        RenderSimpleMesh(meshes["tank_body_1"], shaders["TemaShader"], modelMatrix);
    }
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(inamic_x, 0, inamic_z));
        RenderSimpleMesh(meshes["tank_body_2"], shaders["TemaShader"], modelMatrix);
    }
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(inamic_x, 0, inamic_z));
        modelMatrix = glm::translate(modelMatrix, glm::vec3(heigth_leg / 2 + distance_legs / 2, heigth_leg + 1.25 * heigth_body, 0.05f));
        RenderSimpleMesh(meshes["tank_cilindru"], shaders["TemaShader"], modelMatrix);
    }
    inamic_z -= 0.01f;
    
    
    //Cladiri RANDOM
    for (int i = 0; i < 25; i++) {
        Generate_random_cladire(cladire[i].random_distance_x, cladire[i].height, cladire[i].random_distance_z);
    }
    //Plan
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.1f, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(100));
        RenderSimpleMesh(meshes["plane"], shaders["TemaShader"], modelMatrix);
    }
}


void Tema2::FrameEnd()
{
    //DrawCoordinateSystem();
    DrawCoordinateSystem(camera_tema->GetViewMatrix(), projectionMatrix);
}


void Tema2::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    /*glm::mat4 eyePosition = camera_tema->GetViewMatrix();
    GLint eye_pos = glGetUniformLocation(shader->program, "eye_position");
    glUniformMatrix4fv(eye_pos, 1, GL_FALSE, glm::value_ptr(eyePosition));*/

    // TODO(student): Get shader location for uniform mat4 "Model"
    int shader_location1 = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(shader_location1, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // TODO(student): Get shader location for uniform mat4 "View"
    glm::mat4 viewMatrix = camera_tema->GetViewMatrix();
    int shader_location2 = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(shader_location2, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // TODO(student): Get shader location for uniform mat4 "Projection"
    projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);
    int shader_location3 = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(shader_location3, 1, GL_FALSE, glm::value_ptr(projectionMatrix));


    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}



void Tema2::OnInputUpdate(float deltaTime, int mods)
{
    // move the camera only if MOUSE_RIGHT button is pressed
    
        float cameraSpeed = 2.0f;

        if (window->KeyHold(GLFW_KEY_W)) {
            camera_tema->MoveForward(deltaTime * cameraSpeed);
        }
       

        if (window->KeyHold(GLFW_KEY_A)) {
            // TODO(student): Translate the camera to the left
            camera_tema->RotateThirdPerson_OY(deltaTime * cameraSpeed);
            rotate_tank += deltaTime * cameraSpeed;
        }

        if (window->KeyHold(GLFW_KEY_S)) {
            // TODO(student): Translate the camera backward
            camera_tema->MoveForward(-deltaTime * cameraSpeed);
        }

        if (window->KeyHold(GLFW_KEY_D)) {
            // TODO(student): Translate the camera to the right
            camera_tema->RotateThirdPerson_OY(- deltaTime * cameraSpeed);
            rotate_tank -= deltaTime * cameraSpeed;
        }
}


void Tema2::OnKeyPress(int key, int mods)
{
    // Add key press event
    
    // TODO(student): Switch projections

}


void Tema2::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
}


void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
    if (button == 1) {
        shoot += 1;
    }

}


void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema2::OnWindowResize(int width, int height)
{
}
