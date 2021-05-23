
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
	Mesh* sky_mesh;
	Texture* sky_tex;
	Matrix44 sky_model;

	World(int x);
	void inicializePenguins();
	void renderPenguins(bool renderBoundings);
	void inicializeBlocks();
	void renderBlocks(bool renderBoundings);
	void inicializePlayer();
	void renderPlayer();
	void renderSky();
	bool isPlayeronaBlock(Vector3 playerpos);
	void BlockVibration(int block, float et);
};