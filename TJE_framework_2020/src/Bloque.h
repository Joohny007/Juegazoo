#pragma once
#include "framework.h"
#include "mesh.h"
#include "texture.h"

class Bloque
{
public:
    //virtual ~Player(); //DESTRUCTOR MASIVO

    //some attributes 
    Matrix44 model;
    Mesh* mesh;
    Texture* texture;
    Shader* shader;
    Vector4 color;

    Bloque(int x); //constructor
    void inicialize();
    void render();
};