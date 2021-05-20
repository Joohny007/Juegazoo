#pragma once
#include "framework.h"
#include "mesh.h"
#include "texture.h"
#include "shader.h"
#include "camera.h"

class Penguin
{
public:

	int id;
    Matrix44 model;
    Mesh* mesh;
    Texture* texture;
    Shader* shader;
    Vector4 color;

	Penguin(int id, Vector3 pos);

    void inicialize();
    void render();

};