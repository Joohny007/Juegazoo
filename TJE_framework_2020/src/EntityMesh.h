#pragma once
#include "mesh.h"
#include "texture.h"
#include "Entity.h"
#include "camera.h"
#include "shader.h"

class EntityMesh : public Entity
{
public:
    //Attributes of this class 
    Mesh* mesh;
    Texture* texture;
    Shader* shader;
    Vector4 color;

    //methods overwritten 
    void render();
    void update(float dt);
};