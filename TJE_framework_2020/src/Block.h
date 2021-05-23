#pragma once

#include "framework.h"
#include "mesh.h"
#include "texture.h"
#include "camera.h"
#include "shader.h"

class Block
{
public:

    //some attributes 
    int id;
    Matrix44 model;
    Mesh* mesh;
    Texture* texture;
    Shader* shader;
    Vector4 color;
    enum {
        UP,
        DOWN
    } dir;
    float vel;

    Block(int x, Vector3 pos); //constructor
    void inicialize();
    void render();
    void vibrate(float et);
};