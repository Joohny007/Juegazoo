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
    int id;
    float speed;
    Vector3 playerSpeed;
    float rot_speed;
    float yaw;
    float dir;
    bool kick;
    bool moving;
    float moving_counter;
    float kick_counter;
    float stun_counter;
    float kick_cooldown;
    bool stunned;
    Vector3 pos;
    Matrix44 model;
    Matrix44 rot;
    Mesh* mesh;
    Texture* texture;
    Shader* shader;
    Vector4 color;
;
    Player();
    Player(int id); //constructor
    void setPos(Vector3 pos);
    void inicialize();
    void inicialize2();
    void render(Camera* camera);
    bool penguinCollision();
};