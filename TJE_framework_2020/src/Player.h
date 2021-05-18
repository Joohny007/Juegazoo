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
    //some attributes 
    Matrix44 model;
    Mesh* mesh;
    Texture* texture;
    Shader* shader;
    Vector4 color;

    Player(); //constructor
    virtual ~Player(); //DESTRUCTOR MASIVO
    void render();
};