#pragma once
#include "framework.h"
#include "mesh.h"
#include "texture.h"
#include "shader.h"
#include "camera.h"

class Penguin
{
public:
    enum type {
        LEFT,
        RIGHT,
        FORWARD,
        BACKWARD
    };
	int id;
    float dir;
    Matrix44 model;
    Mesh* mesh;
    Texture* texture;
    Shader* shader;
    Vector4 color;
    Vector3 pos;

	Penguin(int id, Vector3 pos);

    void inicialize();
    void render(Camera* camera);

};