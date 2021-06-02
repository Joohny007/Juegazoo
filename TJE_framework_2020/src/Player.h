#pragma once
#include <string>
#include "framework.h"
#include "mesh.h"
#include "texture.h"
#include "camera.h"
#include "shader.h"

class Player
{
public:
    //virtual ~Player(); //DESTRUCTOR MASIVO
    enum type {
        LEFT,
        RIGHT,
        FORWARD,
        BACKWARD
    };
    //some attributes 
    float speed;
    float rot_speed;
    float yaw;
    float dir;
    Vector3 pos;
    Matrix44 model;
    Matrix44 rot;
    Mesh* mesh;
    Texture* texture;
    Shader* shader;
    Vector4 color;
;
    Player(); //constructor
    void setPos(Vector3 pos);
    void inicialize();
    void render();
    bool penguinCollision();
};