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
    Matrix44 model;
    Mesh* mesh;
    Texture* texture;
    Shader* shader;
    Vector4 color;

    Player(int x); //constructor
    void inicialize();
    void render();
};