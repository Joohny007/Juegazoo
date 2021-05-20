#include "World.h"

World::World(int x)
{
	this->pingu_counter = 0;
	this->max_penguins = 15;
	this->max_blocks = 15;
	//this->player = Player(2);
}

void World::inicializePenguins()
{
	Vector3 pos = Vector3(1, 0, 0);
	for (int id = 0; id < max_penguins; id++) {
		this->penguins.push_back(Penguin(id, pos));
		pos.x += 5;
		penguins[id].inicialize();
	}
}

void World::renderPenguins()
{
	for (int id = 0; id < max_penguins; id++) {
		this->penguins[id].render();
	}
}

void World::inicializeBlocks()
{
	int counter = 0;
	Vector3 pos = Vector3(1, 0, 0);
	for (int id = 0; id < max_blocks; id++) {
		this->blocks.push_back(Block(id, pos));
		pos.x += 5;
		blocks[id].inicialize();
	}
}

void World::renderBlocks()
{
	for (int id = 0; id < max_blocks; id++) {
		this->blocks[id].render();
	}
}