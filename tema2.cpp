#include "lab_m1/tema2/tema2.h"
#include "lab_m1/tema2/tema2_transform_3d.h"
#include <GL/glew.h>

#include <vector>
#include <iostream>
#include <string>
#include <math.h>

#include "core/engine.h"
#include "utils/gl_utils.h"

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


int start_shooting = 0, second_mouseX = 0, been_hit = 0;
int case1 = 0, case2 = 0, case3 = 0, pressing_smth = 0;;
int speed = 0, distance_traveled = 0;
int movex = 0, movey = 0, movez = 0, signal = 0;
int move_forward = 0, move_backwards = 0, move_left = 0, move_right = 0;
float reload_time = 0, angle = 0;
float tank_radius = 1, building_radius = 5, time_to_fade_away = 0;
float time_to_disappear;
float translateZ_new_enemy;
float translateX_new_enemy;
int counting = 0, second_deltaX = 0, player_radius = 15, index = 0, old_pos;
float radius = 1.0f, cannonball_radius = 0.3f;
float centerX = -0.5f, directionX, directionZ, magnitude, curren_pos_cannonball_x = -0.5f, current_pos_cannonball_z = 0.5f;
float centerX_enemy, centerZ_enemy, directionX_enemy, directionZ_enemy, cannonball_x_enemy, cannonball_z_enemy, magnitude_enemy, shoot_time_enemy = 0, distance_traveled_enemy = 0;
float centerZ = 0.5f, rotate_left = 0, rotate_right = 0;
float time_enemies = 0, ox_enemy1 = 0, oy_enemy1 = 0, oz_enemy1 = 0, ox_enemy2 = 0, oy_enemy2 = 0, oz_enemy2 = 0, time_translatex = 0, time_translatey = 0, time_translatez = 0;
float translatex = 0, translatey = 0, translatez = 0, rotate_turret = 0;
bool turret_alligned = false, turret_alligned_1 = false, hit_one = false, hit_two = false;
float second_translatex = 1, second_translatey = 1, second_translatez = 1;
float directionOX, directionOZ, angleOX, angleOZ, old_rotate_turret;


std::vector<Mesh*>ammo;
std::vector<Mesh*>ammo_enemy;
std::vector<int>indices_to_remove;
std::vector<std::tuple<float, float, float>>enemies_coord;
std::vector<int>speed_cannonball;
std::vector<int>speed_cannonball_enemy;
std::vector<float>time_substitutes;

struct enemy {
    float translateX_enemy = 0;
    float translateY_enemy = 0;
    float translateZ_enemy = 0;
    float ox_enemy = 0;
    float oy_enemy = 0;
    float oz_enemy = 0;
    float turret_rotation = 0;
    bool turret_alligned = false;
    int enemy_radius = 0;
    int been_hit = 0;
    int start_shooting = 0;
    float cannonball_traveled_distance = 0;
    float shoot_time_enemy = 0;

};

std::vector<enemy> enemies_tanks;


//Shader* shader;


tema2::tema2()
{
}


tema2::~tema2()
{
}


void tema2::Init()
{
    cullFace = GL_BACK;
    polygonMode = GL_FILL;

    // Load a mesh from file into GPU memory
    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    translateX = 0;
    translateY = 0;
    translateZ = 0;

    angularStepOX = 0;
    angularStepOY = 0;
    angularStepOZ = 0;
    angularStepOY_2 = 0;

    //projectiles
    {
        Mesh* sphere = new Mesh("projectile");
        sphere->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[sphere->GetMeshID()] = sphere;

    }

    {
        Mesh* trapese = new Mesh("trapese");
        trapese->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "trapese.obj");
        meshes[trapese->GetMeshID()] = trapese;
    }

    {
        Mesh* barrell = new Mesh("barrell");
        barrell->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "barrell.obj");
        meshes[barrell->GetMeshID()] = barrell;
    }

    {
        Mesh* base = new Mesh("base");
        base->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "base.obj");
        meshes[base->GetMeshID()] = base;
    }

    {
        Mesh* cannonball = new Mesh("cannonball");
        cannonball->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[cannonball->GetMeshID()] = cannonball;
    }


    {
        vector<VertexFormat> vertices
        {
            VertexFormat(glm::vec3(-3, -1,  0), glm::vec3(0, 1, 0), glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(1, -1,  0), glm::vec3(0, 1, 0), glm::vec3(0.2, 0.1, 0.2)),
            VertexFormat(glm::vec3(-2, 0,  0), glm::vec3(0, 1, 0), glm::vec3(0.2, 0.0, 0.3)),
            VertexFormat(glm::vec3(0, 0,  0), glm::vec3(0, 1, 0), glm::vec3(0.1, 0.8, 0.6)),
            VertexFormat(glm::vec3(-3, -1,  2), glm::vec3(0, 1, 0), glm::vec3(0.1, 0.7, 0.4)),
            VertexFormat(glm::vec3(1, -1,  2), glm::vec3(0, 1, 0), glm::vec3(0.4, 0.9, 0.2)),
            VertexFormat(glm::vec3(-2, 0,  2), glm::vec3(0, 1, 0), glm::vec3(0.4, 0.98, 0.4)),
            VertexFormat(glm::vec3(0, 0,  2), glm::vec3(0, 1, 0), glm::vec3(0.1, 0.5, 0.1))
            // Complete the vertices data for the cube mesh

        };

        vector<unsigned int> indices =
        {
            // Complete indices data for the cube mesh
            0, 1, 2,
            1, 3, 2,
            2, 3, 7,
            2, 7, 6,
            1, 7, 3,
            1, 5, 7,
            6, 7, 4,
            7, 5, 4,
            0, 4, 1,
            1, 4, 5,
            2, 6, 4,
            0, 2, 4

        };


        meshes["cube_A"] = new Mesh("generated cube 1");
        meshes["cube_A"]->InitFromData(vertices, indices);

        //turret(barrel)


        vector<VertexFormat> vertices3
        {
            VertexFormat(glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.2, 0.8, 0.6)), //0
            VertexFormat(glm::vec3(-0.5f, -0.5f,  -1.5f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.2, 0.1, 0.2)), // 1
            VertexFormat(glm::vec3(0.5f, -0.5f,  -1.5f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.2, 0.0, 0.3)), // 2
            VertexFormat(glm::vec3(0.5f, -0.5f,  0.5f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.1, 0.8, 0.6)), // 3
            VertexFormat(glm::vec3(-0.5f, 0.5f,  0.5f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.1, 0.7, 0.4)), // 4
            VertexFormat(glm::vec3(0.5f, 0.5f,  0.5f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.4, 0.9, 0.2)), // 5
            VertexFormat(glm::vec3(0.5f, 0.5f,  -1.5f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.4, 0.98, 0.4)), // 6
            VertexFormat(glm::vec3(-0.5f, 0.5f,  -1.5f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.1, 0.5, 0.1)) // 7
            // Complete the vertices data for the cube mesh

        };

        vector<unsigned int> indices3 =
        {
            // Complete indices data for the cube mesh
            0, 1, 2,
            1, 3, 2,
            2, 3, 7,
            2, 7, 6,
            1, 7, 3,
            1, 7, 5,
            6, 7, 4,
            7, 5, 4,
            0, 4, 1,
            1, 4, 5,
            2, 6, 4,
            0, 2, 4

        };

        meshes["cube_B"] = new Mesh("generated cube 3");
        meshes["cube_B"]->InitFromData(vertices3, indices3);



        //body (tank)
        vector<VertexFormat> vertices2
        {
            VertexFormat(glm::vec3(-2, -0.5f,  0), glm::vec3(0.6f, 0.4f, 0.0f), glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(0, -0.5f,  0), glm::vec3(0.6f, 0.4f, 0.0f), glm::vec3(0.2, 0.1, 0.2)),
            VertexFormat(glm::vec3(-2, 0,  0), glm::vec3(0.6f, 0.4f, 0.0f), glm::vec3(0.2, 0.0, 0.3)),
            VertexFormat(glm::vec3(0, 0,  0), glm::vec3(0.6f, 0.4f, 0.0f), glm::vec3(0.1, 0.8, 0.6)),
            VertexFormat(glm::vec3(-2, -0.5f,  2), glm::vec3(0.6f, 0.4f, 0.0f), glm::vec3(0.1, 0.7, 0.4)),
            VertexFormat(glm::vec3(0, -0.5f,  2), glm::vec3(0.6f, 0.4f, 0.0f), glm::vec3(0.4, 0.9, 0.2)),
            VertexFormat(glm::vec3(-2, 0,  2), glm::vec3(0.6f, 0.4f, 0.0f), glm::vec3(0.4, 0.98, 0.4)),
            VertexFormat(glm::vec3(0, 0,  2), glm::vec3(0.6f, 0.4f, 0.0f), glm::vec3(0.1, 0.5, 0.1))
            // Complete the vertices data for the cube mesh

        };

        vector<unsigned int> indices2 =
        {
            // Complete indices data for the cube mesh
            0, 1, 2,
            1, 3, 2,
            2, 3, 7,
            2, 7, 6,
            1, 7, 3,
            1, 5, 7,
            6, 7, 4,
            7, 5, 4,
            0, 4, 1,
            1, 4, 5,
            2, 6, 4,
            0, 2, 4

        };

        meshes["cube_C"] = new Mesh("generated cube 2");
        meshes["cube_C"]->InitFromData(vertices2, indices2);





        CreateMesh("cube_C", vertices2, indices2);
    }

    {
        vector<VertexFormat> vertices1
        {
            VertexFormat(glm::vec3(1, 0, 0), glm::vec3(1, 1, 1), glm::vec3(0.2, 0.1, 0.2)),
            VertexFormat(glm::vec3(1.5f, 0, 1), glm::vec3(0, 0, 1), glm::vec3(0.2, 0.0, 0.3)),
            VertexFormat(glm::vec3(2, 0, 0), glm::vec3(1, 0, 1), glm::vec3(0.1, 0.7, 0.4)),
            VertexFormat(glm::vec3(1.5f, 1.5f, 0), glm::vec3(1, 1, 1), glm::vec3(0.1, 0.5, 0.4))
        };

        vector<unsigned int> indices_1 = {
            1, 3, 0,
            2, 3, 1,
            2, 1, 0,
            3, 2, 0
        };

        meshes["quad"] = new Mesh("generated quad");
        meshes["quad"]->InitFromData(vertices1, indices_1);

    }

    {
        vector<VertexFormat> vertices2
        {
            VertexFormat(glm::vec3(1, 0, 0), glm::vec3(1, 1, 1), glm::vec3(0.2, 0.1, 0.2)),
            VertexFormat(glm::vec3(2, 0, 0), glm::vec3(0, 0, 1), glm::vec3(0.2, 0.0, 0.3)),
            VertexFormat(glm::vec3(1, 1, 0), glm::vec3(1, 0, 1), glm::vec3(0.1, 0.7, 0.4)),
            VertexFormat(glm::vec3(2, 1, 0), glm::vec3(1, 1, 1), glm::vec3(0.1, 0.5, 0.4))
        };

        vector<unsigned int> indices_2 = {
            0, 1, 3,
            0, 2, 3
        };

        meshes["square"] = new Mesh("generated square");
        meshes["square"]->InitFromData(vertices2, indices_2);

    }

    {
        int nr_triangles = 38;
        vector<VertexFormat>circle_vertices;
        circle_vertices.push_back(VertexFormat(glm::vec3(0, 0, 0), glm::vec3(0, 0, 1), glm::vec3(0.2, 0.8, 0.6)));
        for (int i = 0; i < nr_triangles; i++) {
            circle_vertices.push_back(VertexFormat(glm::vec3((glm::cos((i * 10 + 10) * (glm::pi<float>() / 180))), (glm::sin((i * 10 + 10) * (glm::pi<float>() / 180))), 0), glm::vec3(0, 0, 1), glm::vec3(0.2, 0.8, 0.6)));
        }

        vector<unsigned int>circle_indices;
        for (int i = 0; i < nr_triangles; i++) {
            circle_indices.push_back(i);
        }

        meshes["circle"] = new Mesh("generated circle 1");
        meshes["circle"]->SetDrawMode(GL_TRIANGLE_FAN);
        meshes["circle"]->InitFromData(circle_vertices, circle_indices);

    }
}


void tema2::CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned int>& indices)
{
    unsigned int VAO = 0;
    // TODO(student): Create the VAO and bind it
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    unsigned int VBO = 0;
    // TODO(student): Create the VBO and bind it
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // TODO(student): Send vertices data into the VBO buffer

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

    unsigned int IBO = 0;
    // TODO(student): Create the IBO and bind it
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

    // TODO(student): Send indices data into the IBO buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

    // ========================================================================
    // This section demonstrates how the GPU vertex shader program
    // receives data. It will be learned later, when GLSL shaders will be
    // introduced. For the moment, just think that each property value from
    // our vertex format needs to be sent to a certain channel, in order to
    // know how to receive it in the GLSL vertex shader.

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

    // TODO(student): Unbind the VAO
    glBindVertexArray(0);

    // Check for OpenGL errors
    if (GetOpenGLError() == GL_INVALID_OPERATION)
    {
        //cout << "\t[NOTE] : For students : DON'T PANIC! This error should go away when completing the tasks." << std::endl;
        //cout << "\t[NOTE] : For developers : This happens because OpenGL core spec >=3.1 forbids null VAOs." << std::endl;
    }

    // Mesh information is saved into a Mesh object
    meshes[name] = new Mesh(name);
    meshes[name]->InitFromBuffer(VAO, static_cast<unsigned int>(indices.size()));
}


void tema2::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(1, 1, 1, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();

    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void tema2::Update(float deltaTimeSeconds)
{
    speed += 100 * deltaTimeSeconds;
    glLineWidth(3);
    glPointSize(5);
    glPolygonMode(GL_FRONT_AND_BACK, polygonMode);
    if (time_enemies >= 300) {
        enemy enemy_1;
        enemy_1.ox_enemy = -3;
        enemy_1.oy_enemy = 0.4f;
        enemy_1.oz_enemy = -3;
        enemy_1.translateX_enemy = 0;
        enemy_1.translateY_enemy = 0;
        enemy_1.translateZ_enemy = 0;
        enemy_1.turret_rotation = 0;
        enemy_1.enemy_radius = 15;
        enemy_1.turret_alligned = false;
        enemy_1.been_hit = 0;
        enemy_1.cannonball_traveled_distance = 0;
        enemy_1.shoot_time_enemy = 0;
        enemy_1.start_shooting = 0;
        if (enemies_tanks.empty()) {
            enemies_tanks.push_back(enemy_1);
        }
        
        time_substitutes.push_back(0);
        case1 = 1;
        time_enemies = 0;

    }


    // Render an object using colors from vertex
    

    if (been_hit == 0) {
        // draw turret(body)
        modelMatrix = glm::mat4(1);
        modelMatrix *= tema2_transform_3d::Translate(-0.5f + translateX, 0.4f, 0.5f + translateZ);
        modelMatrix *= tema2_transform_3d::RotateOY(-angle);
        modelMatrix *= tema2_transform_3d::RotateOY(angularStepOY / 100);
        modelMatrix *= tema2_transform_3d::Scale(0.35f, 0.35f, 0.35f);
        RenderMesh(meshes["trapese"], shaders["Green"], modelMatrix);
        modelMatrix *= tema2_transform_3d::Translate(0.0f, 0.2f, 1.55f);
        modelMatrix *= tema2_transform_3d::Scale(0.7f, 0.7f, 0.7f);
        RenderMesh(meshes["barrell"], shaders["Blue"], modelMatrix);

        // draw tank(body)

        modelMatrix = glm::mat4(1);
        modelMatrix *= tema2_transform_3d::Translate(-0.5f + translateX, 0.07f, 0.5f + translateZ);
        modelMatrix *= tema2_transform_3d::RotateOY(-angle);
        modelMatrix *= tema2_transform_3d::Scale(0.4f, 0.3f, 0.3f);
        RenderMesh(meshes["base"], shaders["Brown"], modelMatrix);

    }
    else if(been_hit == 1) {
        // draw turret(body)
        modelMatrix = glm::mat4(1);
        modelMatrix *= tema2_transform_3d::Translate(-0.5f + translateX, 0.4f, 0.5f + translateZ);
        modelMatrix *= tema2_transform_3d::RotateOY(-angle);
        modelMatrix *= tema2_transform_3d::RotateOY(angularStepOY / 100);
        modelMatrix *= tema2_transform_3d::Scale(0.35f, 0.35f, 0.35f);
        RenderMesh(meshes["trapese"], shaders["Brown"], modelMatrix);
        modelMatrix *= tema2_transform_3d::Translate(0.0f, 0.2f, 1.55f);
        modelMatrix *= tema2_transform_3d::Scale(0.7f, 0.7f, 0.7f);
        RenderMesh(meshes["barrell"], shaders["Blue"], modelMatrix);

        // draw tank(body)

        modelMatrix = glm::mat4(1);
        modelMatrix *= tema2_transform_3d::Translate(-0.5f + translateX, 0.07f, 0.5f + translateZ);
        modelMatrix *= tema2_transform_3d::RotateOY(-angle);
        modelMatrix *= tema2_transform_3d::Scale(0.4f, 0.3f, 0.3f);
        RenderMesh(meshes["base"], shaders["VertexColor"], modelMatrix);

    }
    else if (time_to_fade_away > 53 && time_to_fade_away <= 56) {
        cout << "switch color !!! \n";
        // draw turret(body)
        modelMatrix = glm::mat4(1);
        modelMatrix *= tema2_transform_3d::Translate(-0.5f + translateX, 0.4f, 0.5f + translateZ);
        modelMatrix *= tema2_transform_3d::RotateOY(-angle);
        modelMatrix *= tema2_transform_3d::RotateOY(angularStepOY / 100);
        modelMatrix *= tema2_transform_3d::Scale(0.35f, 0.35f, 0.35f);
        RenderMesh(meshes["trapese"], shaders["Brown"], modelMatrix);
        modelMatrix *= tema2_transform_3d::Translate(0.0f, 0.2f, 1.55f);
        modelMatrix *= tema2_transform_3d::Scale(0.7f, 0.7f, 0.7f);
        RenderMesh(meshes["barrell"], shaders["VertexColor"], modelMatrix);

        // draw tank(body)

        modelMatrix = glm::mat4(1);
        modelMatrix *= tema2_transform_3d::Translate(-0.5f + translateX, 0.07f, 0.5f + translateZ);
        modelMatrix *= tema2_transform_3d::RotateOY(-angle);
        modelMatrix *= tema2_transform_3d::Scale(0.4f, 0.3f, 0.3f);
        RenderMesh(meshes["base"], shaders["VertexColor"], modelMatrix);

    }
    else if (time_to_fade_away >= 100) {
        // draw turret(body)
        modelMatrix = glm::mat4(1);
        modelMatrix *= tema2_transform_3d::Translate(-0.5f + translateX, 0.4f, 0.5f + translateZ);
        modelMatrix *= tema2_transform_3d::RotateOY(-angle);
        modelMatrix *= tema2_transform_3d::RotateOY(angularStepOY / 100);
        modelMatrix *= tema2_transform_3d::Scale(0.35f, 0.35f, 0.35f);
        RenderMesh(meshes["trapese"], shaders["Vertex Color"], modelMatrix);
        modelMatrix *= tema2_transform_3d::Translate(0.0f, 0.2f, 1.55f);
        modelMatrix *= tema2_transform_3d::Scale(0.7f, 0.7f, 0.7f);
        RenderMesh(meshes["barrell"], shaders["VertexColor"], modelMatrix);

        // draw tank(body)

        modelMatrix = glm::mat4(1);
        modelMatrix *= tema2_transform_3d::Translate(-0.5f + translateX, 0.07f, 0.5f + translateZ);
        modelMatrix *= tema2_transform_3d::RotateOY(-angle);
        modelMatrix *= tema2_transform_3d::Scale(0.4f, 0.3f, 0.3f);
        RenderMesh(meshes["base"], shaders["VertexColor"], modelMatrix);

    }
    
    if (start_shooting == 1) {
        for (int i = 0; i < ammo.size(); i++) {
            if ((distance_traveled <= 55 || signal == 0)) {
                float shooting_angle = angularStepOY / 100;
                modelMatrix = glm::mat4(1);
                if (rotate_left == 1) {
                    centerX = -0.5f - angle;
                    centerZ = 0.5f - angle;

                    shooting_angle = (angularStepOY / 100) - angle;

                    float directionX = (-1) * sin(-shooting_angle);
                    float directionZ = (-1) * cos(shooting_angle);

                    curren_pos_cannonball_x = (-0.5f) + directionX * speed_cannonball[i];
                    current_pos_cannonball_z = (0.5f) + directionZ * speed_cannonball[i];

                }

                if (rotate_right == 1) {
                    centerX = -0.5f + angle;
                    centerZ = 0.5f + angle;

                    shooting_angle = (angularStepOY / 100) - angle;
                    float directionX = (-1) * sin(-shooting_angle);
                    float directionZ = (-1) * cos(shooting_angle);

                    curren_pos_cannonball_x = (-0.5f) + directionX * speed_cannonball[i];
                    current_pos_cannonball_z = (0.5f) + directionZ * speed_cannonball[i];

                }

                if (rotate_left == 0 && rotate_right == 0) {
                    centerX = -0.5f;
                    centerZ = 0.5f;

                    curren_pos_cannonball_x = -0.5f;
                    current_pos_cannonball_z = 0.5f;

                    float posX = centerX + sin((-1) * shooting_angle);
                    float posZ = centerZ + cos(shooting_angle);

                    directionX = posX - curren_pos_cannonball_x;
                    directionZ = posZ - current_pos_cannonball_z;
                    magnitude = sqrt(directionX * directionX + directionZ * directionZ);

                    directionX /= magnitude;
                    directionZ /= magnitude;

                    curren_pos_cannonball_x += (-1) * directionX * speed_cannonball[i];
                    current_pos_cannonball_z += (-1) * directionZ * speed_cannonball[i];
                }
                modelMatrix *= tema2_transform_3d::Translate((curren_pos_cannonball_x)+translateX, 0.4, (current_pos_cannonball_z)+translateZ);
                modelMatrix *= tema2_transform_3d::Scale(0.15f, 0.15f, 0.15f);
                RenderMesh(ammo[i], shaders["Brown"], modelMatrix);
                speed_cannonball[i] -= 100 * deltaTimeSeconds;
                distance_traveled += 100 * deltaTimeSeconds;

            }
        }
    }

    for (int i = 0; i < enemies_tanks.size(); i++) {
        float x = enemies_tanks[i].ox_enemy;
        float y = enemies_tanks[i].oy_enemy;
        float z = enemies_tanks[i].oz_enemy;
        float translatex = enemies_tanks[i].translateX_enemy;
        float translatey = enemies_tanks[i].translateY_enemy;
        float translatez = enemies_tanks[i].translateZ_enemy;

    if (time_translatex > 160 && time_translatex < 210 && enemies_tanks[i].been_hit == 0) {
        translatex += deltaTimeSeconds;
        enemies_tanks[i].translateX_enemy = translatex;
    }

    if ((tank_radius + 1 > glm::distance(0.5f + translateZ, z + translatez)) || (tank_radius + 1 > glm::distance(-0.5f + translateX, x + translatex)) && enemies_tanks[i].been_hit == 0) {
        directionOX = translateX - 0.5f - (translatex + x);
        directionOZ = translateZ + 0.5f - (translatez + z);

        float interpolationFactor = 0.05f;
        float angleToPlayer = atan2(directionOZ, directionOX);

        if (translateX < translatex) {
            angleToPlayer += glm::pi<float>();
        }
        else {
            angleToPlayer -= glm::pi<float>();
        }

        angleToPlayer += 2 * glm::pi<float>();
        angleToPlayer = fmod(angleToPlayer, 2 * glm::pi<float>());

        rotate_turret += (angleToPlayer + 1.5f - rotate_turret) * (interpolationFactor);
        enemies_tanks[i].start_shooting = 1;

        if (time_substitutes[i] == 0) {
            Mesh* cannonball = new Mesh("cannonball");
            cannonball->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
            meshes[cannonball->GetMeshID()] = cannonball;
            ammo_enemy.push_back(cannonball);
            speed_cannonball_enemy.push_back(0);
        }
        
        time_substitutes[i] += 10 * deltaTimeSeconds;
        enemies_tanks[i].shoot_time_enemy = time_substitutes[i];

    }
    else {
        enemies_tanks[i].start_shooting = 0;
    }

    float distance = sqrt(
        pow((curren_pos_cannonball_x - (translatex + x)), 2) +
        pow((current_pos_cannonball_z - (translatez + z)), 2)
    );

    if (distance < tank_radius) {
        enemies_tanks[i].been_hit = 1;
    }

    if (time_translatey > 225 && time_translatey < 270 && enemies_tanks[i].been_hit == 0) {
        translatey += deltaTimeSeconds;
        enemies_tanks[i].translateY_enemy = translatey;
        if (translatey < 0) {
            translatey += 2 * glm::pi<float>();
        }
        if (translatey > 2 * glm::pi<float>()) {
            translatey -= 2 * glm::pi<float>();
        }

        if (translatey == 0) {
            second_translatez += deltaTimeSeconds * sin(translatey + 1.5f * glm::pi<float>());
        }
        else {
            second_translatez -= deltaTimeSeconds * sin(translatey + 1.5f * glm::pi<float>());
        }

        second_translatex += deltaTimeSeconds * cos(translatey + 1.5f * glm::pi<float>());

    }

    if (time_translatez > 275 && time_translatez < 320 && enemies_tanks[i].been_hit == 0) {
        translatez += deltaTimeSeconds;
        enemies_tanks[i].translateZ_enemy = translatez;
    }
    else if (time_translatez > 340) {
        time_translatex = 0;
        time_translatey = 0;
        time_translatez = 0;
    }

    if (enemies_tanks[i].been_hit == 1 && time_to_disappear <= 150) {
        modelMatrix = glm::mat4(1);
        modelMatrix *= tema2_transform_3d::Translate(-3 + translatex, 0.4f, -3 + translatez);
        modelMatrix *= tema2_transform_3d::RotateOY(rotate_turret);
        modelMatrix *= tema2_transform_3d::RotateOY(translatey);
        modelMatrix *= tema2_transform_3d::Scale(0.35f, 0.35f, 0.35f);
        RenderMesh(meshes["trapese"], shaders["VertexColor"], modelMatrix);
        modelMatrix *= tema2_transform_3d::Translate(0, 0.2f, 1.55f);
        modelMatrix *= tema2_transform_3d::Scale(0.7f, 0.7f, 0.7f);
        RenderMesh(meshes["barrell"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat4(1);
        modelMatrix *= tema2_transform_3d::Translate(-3 + translatex, 0.2f, -3 + translatez);
        modelMatrix *= tema2_transform_3d::RotateOY(translatey);
        modelMatrix *= tema2_transform_3d::Scale(0.4f, 0.3f, 0.3f);
        RenderMesh(meshes["base"], shaders["VertexColor"], modelMatrix);
        time_to_disappear += 100 * deltaTimeSeconds;
    }
    else {
        modelMatrix = glm::mat4(1);
        modelMatrix *= tema2_transform_3d::Translate(x + translatex, y + 0.05f, z + translatez);
        modelMatrix *= tema2_transform_3d::RotateOY(rotate_turret);
        modelMatrix *= tema2_transform_3d::RotateOY(translatey);
        modelMatrix *= tema2_transform_3d::Scale(0.35f, 0.35f, 0.35f);
        RenderMesh(meshes["trapese"], shaders["Brown"], modelMatrix);
        modelMatrix *= tema2_transform_3d::Translate(x + 3, y - 0.2f, z + 4.55f);
        modelMatrix *= tema2_transform_3d::Scale(0.7f, 0.7f, 0.7f);
        RenderMesh(meshes["barrell"], shaders["Green"], modelMatrix);
        modelMatrix = glm::mat4(1);
        modelMatrix *= tema2_transform_3d::Translate(x + translatex, y - 0.2f, z + translatez);
        modelMatrix *= tema2_transform_3d::RotateOY(translatey);
        modelMatrix *= tema2_transform_3d::Scale(0.4f, 0.3f, 0.3f);
        RenderMesh(meshes["base"], shaders["Blue"], modelMatrix);
    }

    if (enemies_tanks[i].start_shooting == 1) {
        for (int j = 0; j < ammo_enemy.size(); j++) {
            if (time_substitutes[i] >= 100 && enemies_tanks[i].cannonball_traveled_distance <= 55) {
                float shooting_angle_enemy = rotate_turret;
                centerX_enemy = x;
                centerZ_enemy = z;

                cannonball_x_enemy = x;
                cannonball_z_enemy = z;

                float posX_enemy = centerX_enemy + sin((-1) * shooting_angle_enemy);
                float posZ_enemy = centerZ_enemy + cos(shooting_angle_enemy);

                directionX_enemy = posX_enemy - cannonball_x_enemy;
                directionZ_enemy = posZ_enemy - cannonball_z_enemy;
                magnitude_enemy = sqrt(directionX_enemy * directionX_enemy + directionZ_enemy * directionZ_enemy);

                directionX_enemy /= magnitude_enemy;
                directionZ_enemy /= magnitude_enemy;

                cannonball_x_enemy += (-1) * directionX_enemy * speed_cannonball_enemy[j];
                cannonball_z_enemy += (-1) * directionZ_enemy * speed_cannonball_enemy[j];

                modelMatrix *= tema2_transform_3d::Translate((cannonball_x_enemy) + translatex, 0.4, (cannonball_z_enemy) + translatez);
                modelMatrix *= tema2_transform_3d::Scale(1, 1, 1);
                RenderMesh(ammo_enemy[j], shaders["VertexColor"], modelMatrix);
                speed_cannonball_enemy[j] -= 100 * deltaTimeSeconds;
                distance_traveled_enemy += 100 * deltaTimeSeconds;

                float distance_proj = sqrt(
                    pow((cannonball_x_enemy - (translateX - 3)), 2) +
                    pow((cannonball_z_enemy - (translateZ - 3)), 2)
                );

                if (distance_proj < tank_radius && hit_one == false) {
                    been_hit = 1;
                    hit_one = true;
                    counting++;
                    time_to_fade_away += 10 * deltaTimeSeconds;
                }
                cout << "faded : " << time_to_fade_away << " \n";
                if (time_to_fade_away != 0) {
                    time_to_fade_away += 100 * deltaTimeSeconds;
                }
                if (time_to_fade_away > 150) {
                    been_hit = 2;
                }
                enemies_tanks[i].cannonball_traveled_distance += 100 * deltaTimeSeconds;
            }

            if(time_substitutes[i] >= 110) { 
                time_substitutes[i] = 0;
                if (distance_traveled_enemy > 55 || been_hit != 0) {
                    ammo_enemy.pop_back();
                    speed_cannonball_enemy.pop_back();
                }
                enemies_tanks[i].cannonball_traveled_distance = 0;

            }
        }
    }



}

for (int i = enemies_tanks.size() - 1; i >= 0; --i) {
    if (enemies_tanks[i].been_hit == 1 && time_to_disappear > 150) {
        enemies_tanks.erase(enemies_tanks.begin() + i);
        time_to_disappear = 0;
    }
}
    // draw buildings

    // building 1
    modelMatrix = glm::mat4(1);
    modelMatrix *= tema2_transform_3d::Translate(7, 1.82f, 7);
    modelMatrix *= tema2_transform_3d::Scale(3, 3, 3);
    RenderMesh(meshes["box"], shaders["Red"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix *= tema2_transform_3d::Translate(7, 1.82f, 7);
    modelMatrix *= tema2_transform_3d::Scale(0.5f, 3.5f, 3.5f);
    RenderMesh(meshes["box"], shaders["Green"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix *= tema2_transform_3d::Translate(7, 1.82f, 7);
    modelMatrix *= tema2_transform_3d::Scale(3.5f, 3.5f, 0.5f);
    RenderMesh(meshes["box"], shaders["Green"], modelMatrix);

    // building 2

    modelMatrix = glm::mat4(1);
    modelMatrix *= tema2_transform_3d::Translate(3, 1.82f, 3);
    modelMatrix *= tema2_transform_3d::Scale(3, 3, 3);
    RenderMesh(meshes["box"], shaders["Red"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix *= tema2_transform_3d::Translate(3, 1.82f, 3);
    modelMatrix *= tema2_transform_3d::Scale(0.5f, 3.5f, 3.5f);
    RenderMesh(meshes["box"], shaders["Green"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix *= tema2_transform_3d::Translate(3, 1.82f, 3);
    modelMatrix *= tema2_transform_3d::Scale(3.5f, 3.5f, 0.5f);
    RenderMesh(meshes["box"], shaders["Green"], modelMatrix);

    // building 3

    modelMatrix = glm::mat4(1);
    modelMatrix *= tema2_transform_3d::Translate(-9, 1.82f, 3);
    modelMatrix *= tema2_transform_3d::Scale(3, 3, 3);
    RenderMesh(meshes["box"], shaders["Red"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix *= tema2_transform_3d::Translate(-9, 1.82f, 3);
    modelMatrix *= tema2_transform_3d::Scale(0.5f, 3.5f, 3.5f);
    RenderMesh(meshes["box"], shaders["Green"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix *= tema2_transform_3d::Translate(-9, 1.82f, 3);
    modelMatrix *= tema2_transform_3d::Scale(3.5f, 3.5f, 0.5f);
    RenderMesh(meshes["box"], shaders["Green"], modelMatrix);

    // building 4

    modelMatrix = glm::mat4(1);
    modelMatrix *= tema2_transform_3d::Translate(-12, 1.82f, -1);
    modelMatrix *= tema2_transform_3d::Scale(3, 3, 3);
    RenderMesh(meshes["box"], shaders["Red"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix *= tema2_transform_3d::Translate(-12, 1.82f, -1);
    modelMatrix *= tema2_transform_3d::Scale(0.5f, 3.5f, 3.5f);
    RenderMesh(meshes["box"], shaders["Green"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix *= tema2_transform_3d::Translate(-12, 1.82f, -1);
    modelMatrix *= tema2_transform_3d::Scale(3.5f, 3.5f, 0.5f);
    RenderMesh(meshes["box"], shaders["Green"], modelMatrix);

    // building 5

    modelMatrix = glm::mat4(1);
    modelMatrix *= tema2_transform_3d::Translate(-18, 1.82f, -3);
    modelMatrix *= tema2_transform_3d::Scale(3, 3, 3);
    RenderMesh(meshes["box"], shaders["Red"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix *= tema2_transform_3d::Translate(-18, 1.82f, -3);
    modelMatrix *= tema2_transform_3d::Scale(0.5f, 3.5f, 3.5f);
    RenderMesh(meshes["box"], shaders["Green"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix *= tema2_transform_3d::Translate(-18, 1.82f, -3);
    modelMatrix *= tema2_transform_3d::Scale(3.5f, 3.5f, 0.5f);
    RenderMesh(meshes["box"], shaders["Green"], modelMatrix);
    time_enemies += 10 * deltaTimeSeconds;
    time_translatex += 10 * deltaTimeSeconds;
    time_translatey += 10 * deltaTimeSeconds;
    time_translatez += 10 * deltaTimeSeconds;



}


void tema2::FrameEnd()
{
    DrawCoordinateSystem();
    if (been_hit == 2) {
        window->Close();
    }
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void tema2::OnInputUpdate(float deltaTime, int mods)
{
    float distance_building_1 = sqrt(
        pow(((translateX - 3) - 7), 2) +
        pow(((translateZ - 3) - 7), 2) + 
        pow((0.14f - 1.82f), 2)
    );

    float distance_building_2 = sqrt(
        pow(((translateX - 3) - 3), 2) +
        pow(((translateZ - 3) - 3), 2) +
        pow((0.14f - 1.82f), 2)
    );

    float distance_building_3 = sqrt(
        pow(((translateX - 3) - (-9)), 2) +
        pow(((translateZ - 3) - 3), 2)
    );

    float distance_building_4 = sqrt(
        pow(((translateX - 3) - (-12)), 2) +
        pow(((translateZ - 3) - (-1)), 2)
    );

    float distance_building_5 = sqrt(
        pow(((translateX - 3) - (-18)), 2) +
        pow(((translateZ - 3) - (-3)), 2)
    );

        if (window->KeyHold(GLFW_KEY_W) && !(window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)) && been_hit == 0) {
                if (angle == 0) {
                    translateZ += 2 * deltaTime * sin(angle + 1.5f * glm::pi<float>());
                    rotate_left = 0;
                    rotate_right = 0;
                }
                else {
                    translateZ -= 2 * deltaTime * sin(angle + 1.5f * glm::pi<float>());
                }

                translateX += 2 * deltaTime * cos(angle + 1.5f * glm::pi<float>());

            
            move_left = 0;
            move_right = 0;
            move_forward = 1;
            move_backwards = 0;

        }
    

    if (window->KeyHold(GLFW_KEY_S) && !(window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)) && been_hit == 0) {
            if (angle == 0) {
                translateZ -= 2 * deltaTime * sin(angle + 1.5f * glm::pi<float>());
                rotate_left = 0;
                rotate_right = 0;
            }
            else {
                translateZ += 2 * deltaTime * sin(angle + 1.5f * glm::pi<float>());
            }

            translateX -= 2 * deltaTime * cos(angle + 1.5f * glm::pi<float>());
        

        move_left = 0;
        move_right = 0;
        move_forward = 0;
        move_backwards = 1;
    }

    if (window->KeyHold(GLFW_KEY_R) && !(window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))) {
        translateY -= 2 * deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_F) && !(window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))) {
        translateY += 2 * deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_A) && !(window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)) && been_hit == 0) {
        angularStepOY_2 += 2 * deltaTime;
        angle += deltaTime;

        rotate_left = 1;
        move_left = 1;
        move_right = 0;
        move_forward = 0;
        move_backwards = 0;
        rotate_right = 0;

        if (angle > 2 * glm::pi<float>()) {
            angle -= 2 * glm::pi<float>();
        }
    }

    if (window->KeyHold(GLFW_KEY_D) && !(window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)) && been_hit == 0) {
        angularStepOY_2 -= 2 * deltaTime;
        angle -= deltaTime;

        rotate_left = 0;
        rotate_right = 1;
        
        move_left = 0;
        move_right = 1;
        move_forward = 0;
        move_backwards = 0;

        if (angle < 0) {
            angle += 2 * glm::pi<float>();
        }

    }

    if (window->KeyHold(GLFW_KEY_1)) {
        angularStepOX -= 2 * deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_2)) {
        angularStepOX += 2 * deltaTime;
    }



}


void tema2::OnKeyPress(int key, int mods)
{
    // TODO(student): Switch between GL_FRONT and GL_BACK culling.
    // Save the state in `cullFace` variable and apply it in the
    // `Update()` method

    if (key == GLFW_KEY_SPACE)
    {
        switch (polygonMode)
        {
        case GL_POINT:
            polygonMode = GL_FILL;
            break;
        case GL_LINE:
            polygonMode = GL_POINT;
            break;
        default:
            polygonMode = GL_LINE;
            break;
        }
    }
    if (key == GLFW_KEY_F2) {
        if (cullFace == GL_FRONT) {
            cullFace = GL_BACK;
        }
        else if (cullFace == GL_BACK) {
            cullFace = GL_FRONT;
        }
    }

    if (!(window->KeyHold(GLFW_KEY_W) || window->KeyHold(GLFW_KEY_S) ||
        window->KeyHold(GLFW_KEY_R) || window->KeyHold(GLFW_KEY_F) ||
        window->KeyHold(GLFW_KEY_A) || window->KeyHold(GLFW_KEY_D) ||
        window->KeyHold(GLFW_KEY_1) || window->KeyHold(GLFW_KEY_2)) &&
        !(window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))) {

        move_left = 0;
        move_right = 0;
        move_forward = 0;
        move_backwards = 0;
        
    }


}


void tema2::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    if (!window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT) && been_hit == 0) {
        if (angle == 0) {
            angularStepOY = mouseX;
        }
        else {
            angularStepOY = mouseX + angle;

        }

        second_mouseX = mouseX;
    }



}


void tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
    if (button == 1) {
            Mesh* cannonball = new Mesh("cannonball");
            cannonball->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
            meshes[cannonball->GetMeshID()] = cannonball;
            ammo.push_back(cannonball);
            speed_cannonball.push_back(0);
            if (counting == 0) {
                ammo.push_back(cannonball);
                speed_cannonball.push_back(0);
                distance_traveled = 0;
                start_shooting = 1;
                counting++;
            }
            if (distance_traveled >= 50 || signal == 1) {
                ammo.push_back(cannonball);
                speed_cannonball.push_back(0);
                distance_traveled = 0;
                start_shooting = 1;
                counting++;
                signal = 0;
            }

    }


}


void tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void tema2::OnWindowResize(int width, int height)
{
}
