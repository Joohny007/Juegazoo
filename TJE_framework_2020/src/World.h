
#pragma once
#include "mesh.h"
#include "texture.h"
#include "Entity.h"
#include "camera.h"
#include "shader.h"
#include "Player.h"
#include "Block.h"
#include "Penguin.h"

class World 
{
public:
	enum {
		PLAYER,
		ICE,
		PENGUIN,
		BOAT
	} type;

	std::vector<Entity> entities;
	std::vector<Block> blocks;
	std::vector<Penguin> penguins;
	Player player;
	int pingu_counter;
	int max_penguins;
	int max_blocks;

	World(int x);
	void inicializePenguins();
	void renderPenguins();
	void inicializeBlocks();
	void renderBlocks();
	void inicializePlayer();
	void renderPlayer();
	bool isPlayeronaBlock(Vector3 playerpos);
};