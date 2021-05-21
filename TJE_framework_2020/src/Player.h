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

    //some attributes 
    float speed;
    float rot_speed;
    float yaw;
    Vector3 pos;
    Matrix44 model;
    Mesh* mesh;
    Texture* texture;
    Shader* shader;
    Vector4 color;
;
    Player(); //constructor
    void setPos(Vector3 pos);
    void inicialize();
    void render();
};