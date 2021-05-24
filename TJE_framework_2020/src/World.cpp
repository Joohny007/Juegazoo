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
	for (int id = 0; id < max_blocks; id++) {
		this->penguins.push_back(Penguin(id, pos));
		penguins[id].inicialize();
	}
}

void World::renderPenguins(bool renderBoundings)
{
	for (int id = 0; id < max_blocks; id++) {
		Vector3 block_pos = this->blocks[id].model.getTranslation();
		this->penguins[id].render();
		this->penguins[id].model.setTranslation(block_pos.x, block_pos.y + 2, block_pos.z);
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
	this->player.model.setTranslation(10, 2, 5);
}

void World::inicializeSky()
{
	this->sky_mesh = Mesh::Get("data/cielo.ASE");
	this->sky_tex = Texture::Get("data/cielo.tga");
	this->sky_shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
}

void World::renderSky()
{
	//get the last camera that was activated
	Camera* camera = Camera::current;
	Matrix44 model = this->sky_model;
	model.translate(camera->eye.x, camera->eye.y, camera->eye.z);

	//enable shader and pass uniforms

	sky_shader->enable();
	sky_shader->setUniform("u_model", model);
	sky_shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
	sky_shader->setTexture("u_texture", sky_tex->texture_id);


	//render the mesh using the shader
	sky_mesh->render(GL_TRIANGLES);

	//disable the shader after finishing rendering
	sky_shader->disable();
}
void World::inicializeSea()
{
	this->sea_mesh = Mesh::Get("data/agua.ASE");
	this->sea_tex = Texture::Get("data/agua.tga");
	this->sea_shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
}

void World::renderSea()
{
	//get the last camera that was activated
	Camera* camera = Camera::current;
	Matrix44 model = this->sea_model;
	

	//enable shader and pass uniforms

	sea_shader->enable();
	sea_shader->setUniform("u_model", model);
	sea_shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
	sea_shader->setTexture("u_texture", sea_tex->texture_id);


	//render the mesh using the shader
	sea_mesh->render(GL_TRIANGLES);

	//disable the shader after finishing rendering
	sea_shader->disable();
}

float World::isPlayeronaBlock(Vector3 playerpos) {
	/*Vector3 playerpos = players.model.getTranslation();*/
	for (int i = 0; i < blocks.size(); i++) {
		Vector3 blockpos = blocks[i].model.getTranslation();
		/*printf("Blockpos: %f playerpos: %f\n", blockpos.x, playerpos.x);
		printf("Blockpos: %f playerpos: %f\n", blockpos.z, playerpos.z);*/
		if (((blockpos.x - 3.25) <= (playerpos.x+.25)) && ((playerpos.x+.25) <= (blockpos.x + 3.25)) && ((blockpos.z - 2.45) <= (playerpos.z+.25)) && ((playerpos.z+.25) <= (blockpos.z + 2.45))) { return blockpos.y; }
		if (((blockpos.x - 3.25) <= (playerpos.x+.25)) && ((playerpos.x+.25) <= (blockpos.x + 3.25)) && ((blockpos.z - 2.45) <= (playerpos.z-.25)) && ((playerpos.z-.25) <= (blockpos.z + 2.45))) { return blockpos.y; }
		if (((blockpos.x - 3.25) <= (playerpos.x-.25)) && ((playerpos.x-.25) <= (blockpos.x + 3.25)) && ((blockpos.z - 2.45) <= (playerpos.z+.25)) && ((playerpos.z+.25) <= (blockpos.z + 2.45))) { return blockpos.y; }
		if (((blockpos.x - 3.25) <= (playerpos.x-.25)) && ((playerpos.x-.25) <= (blockpos.x + 3.25)) && ((blockpos.z - 2.45) <= (playerpos.z-.25)) && ((playerpos.z-.25) <= (blockpos.z + 2.45))) { return blockpos.y; }
	}
	return -5;
}
void World::BlockVibration(int block, float et) {
	if (this->blocks[block].model.getTranslation().y <= -4) {
		this->max_blocks -= 1;
		this->blocks.erase(this->blocks.begin() + block);
	}
	else {
		this->blocks[block].vibrate(et);
	}

}