#include "World.h"

World::World(int x)
{
	this->pingu_counter = 0;
	this->max_penguins = 15;
	this->max_blocks = 56;
	this->player = Player();
	this->sky_mesh = Mesh::Get("data/cielo.ASE");
	this->sky_tex = Texture::Get("data/cielo.tga");
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

void World::renderPenguins(bool renderBoundings)
{
	for (int id = 0; id < max_penguins; id++) {
		this->penguins[id].render();
		if (renderBoundings) {
			this->penguins[id].mesh->renderBounding(this->penguins[id].model);
		}
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

void World::renderBlocks(bool renderBoundings)
{
	for (int id = 0; id < max_blocks; id++) {
		this->blocks[id].render();
		if (renderBoundings) {
			this->blocks[id].mesh->renderBounding(this->blocks[id].model);
		}
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

void World::renderSky()
{
	//get the last camera that was activated
	Camera* camera = Camera::current;
	this->sky_model.translate(camera->eye.x, camera->eye.y, camera->eye.z);

	//enable shader and pass uniforms
	//if (shader) {
	//	shader->enable();
	//	shader->setUniform("u_model", sky_model);
	//	shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
	//	shader->setTexture("u_texture", sky_tex->texture_id);
	//}

	//render the mesh using the shader
	sky_mesh->render(GL_TRIANGLES);

	//disable the shader after finishing rendering
	//shader->disable();
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
void World::BlockVibration(int block, float et) {
	if (this->blocks[block].model.getTranslation().y <= -4) {
		this->max_blocks -= 1;
		this->blocks.erase(this->blocks.begin() + block);
	}
	this->blocks[block].vibrate(et);

}