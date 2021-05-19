
#pragma once
#include "mesh.h"
#include "texture.h"
#include "Entity.h"
#include "camera.h"
#include "shader.h"

class World 
{
public:
	World();

	enum {
		PLAYER,
		ICE,
		PENGUIN,
		BOAT
	} type;

	std::vector<Entity> entities;
};