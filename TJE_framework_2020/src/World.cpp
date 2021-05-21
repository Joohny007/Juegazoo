#include "World.h"

World::World(int x)
{
	this->pingu_counter = 0;
	this->max_penguins = 15;
	this->max_blocks = 56;
	this->player = Player();
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
		pos.x += 6.5;
		blocks[id].inicialize();
		if (pos.x == (7*6.5) + 1) {
			pos.x = 1;
			pos.z += 4.9;
		}
	}
}

void World::renderBlocks()
{
	for (int id = 0; id < max_blocks; id++) {
		this->blocks[id].render();
	}
}

void World::inicializePlayer()
{
	this->player.inicialize();

}

void World::renderPlayer()
{
	this->player.render();

}
bool World::isPlayeronaBlock(Vector3 playerpos) {
	/*Vector3 playerpos = players.model.getTranslation();*/
	for (int i = 0; i < blocks.size(); i++) {
		Vector3 blockpos = blocks[i].model.getTranslation();
		/*printf("Blockpos: %f playerpos: %f\n", blockpos.x, playerpos.x);
		printf("Blockpos: %f playerpos: %f\n", blockpos.z, playerpos.z);*/
		if (((blockpos.x - 3.25) <= (playerpos.x+.25)) && ((playerpos.x+.25) <= (blockpos.x + 3.25)) && ((blockpos.z - 2.45) <= (playerpos.z+.25)) && ((playerpos.z+1) <= (blockpos.z + 2.45))) { return true; }
		if (((blockpos.x - 3.25) <= (playerpos.x+.25)) && ((playerpos.x+.25) <= (blockpos.x + 3.25)) && ((blockpos.z - 2.45) <= (playerpos.z-.25)) && ((playerpos.z-1) <= (blockpos.z + 2.45))) { return true; }
		if (((blockpos.x - 3.25) <= (playerpos.x-.25)) && ((playerpos.x-.25) <= (blockpos.x + 3.25)) && ((blockpos.z - 2.45) <= (playerpos.z+.25)) && ((playerpos.z+1) <= (blockpos.z + 2.45))) { return true; }
		if (((blockpos.x - 3.25) <= (playerpos.x-.25)) && ((playerpos.x-.25) <= (blockpos.x + 3.25)) && ((blockpos.z - 2.45) <= (playerpos.z-.25)) && ((playerpos.z-1) <= (blockpos.z + 2.45))) { return true; }
	}
	return false;
}
void World::BlockVibration(int block, int dir, float ep){
	Vector3 trans = blocks[block].model.getTranslation();
	trans.y += dir*ep*10.0f;
	blocks[block].model.setTranslation(trans.x,trans.y,trans.z);

}