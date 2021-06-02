#include "World.h"

World::World(int x)
{
	this->pingu_counter = 0;
	this->max_penguins = 15;
	this->max_blocks = 56;
	this->player = Player();
	this->game_time = 0.0f;
}

void World::inicializePenguins()
{
	for (int id = 0; id < max_blocks; id++) {
		Vector3 block_pos = this->blocks[id].model.getTranslation();
		this->penguins.push_back(Penguin(id, block_pos));
		this->penguins[id].model.setTranslation(block_pos.x, block_pos.y + 2, block_pos.z);
		this->penguins[id].model.rotate(180 * DEG2RAD, Vector3(0, 1, 0));
		penguins[id].inicialize();
	}
}

void World::renderPenguins(bool renderBoundings)
{
	for (int id = 0; id < max_blocks; id++) {
		this->penguins[id].render();
		this->penguins[id].pos = this->penguins[id].model.getTranslation();
		//this->penguins[id].model.translate(this->penguins[id].pos.x, this->penguins[id].pos.y, this->penguins[id].pos.z);
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
		if (blocks[i].fallen == true) { continue; }
		if (((blockpos.x - 3.25) <= (playerpos.x+.25)) && ((playerpos.x+.25) <= (blockpos.x + 3.25)) && ((blockpos.z - 2.45) <= (playerpos.z+.25)) && ((playerpos.z+.25) <= (blockpos.z + 2.45))) { return blockpos.y; }
		if (((blockpos.x - 3.25) <= (playerpos.x+.25)) && ((playerpos.x+.25) <= (blockpos.x + 3.25)) && ((blockpos.z - 2.45) <= (playerpos.z-.25)) && ((playerpos.z-.25) <= (blockpos.z + 2.45))) { return blockpos.y; }
		if (((blockpos.x - 3.25) <= (playerpos.x-.25)) && ((playerpos.x-.25) <= (blockpos.x + 3.25)) && ((blockpos.z - 2.45) <= (playerpos.z+.25)) && ((playerpos.z+.25) <= (blockpos.z + 2.45))) { return blockpos.y; }
		if (((blockpos.x - 3.25) <= (playerpos.x-.25)) && ((playerpos.x-.25) <= (blockpos.x + 3.25)) && ((blockpos.z - 2.45) <= (playerpos.z-.25)) && ((playerpos.z-.25) <= (blockpos.z + 2.45))) { return blockpos.y; }
	}
	return -5;
}
void World::BlockVibration(float et) {
	std::vector<int> vect{24, 38, 0, 54, 55, 7, 1, 48, 6, 12, 18, 50, 43, 52, 45, 21, 13, 34, 11, 29, 51, 33, 14, 9, 47, 5, 41, 53, 2, 49, 42, 35, 4, 3, 8, 20, 46, 36, 28, 15, 10, 19, 27, 26, 17, 16, 22, 44, 40, 39, 37, 25, 31, 30};
	game_time += et;
	int size = vect.size();
	if (game_time >= 200) { size -= 0;}
	else if (game_time >= 170) { size -= 2; }
	else if (game_time >= 150) { size -= 4; }
	else if (game_time >= 130) { size -= 8; }
	else if (game_time >= 120) { size -= 12; }
	else if (game_time >= 90.5) { size -= 14; }
	else if (game_time >= 85.5) { size -= 19; }
	else if (game_time >= 70.5) { size -= 22; }
	else if (game_time >= 67.5) { size -= 25; }
	else if (game_time >= 65) { size -= 28; }
	else if (game_time >= 55) { size -= 32; }
	else if (game_time >= 50) { size -= 35; }
	else if (game_time >= 45) { size -= 39; }
	else if (game_time >= 30) { size -= 46; }
	else { size -= 52; }
	for (int block = 0; block < size; block ++){
		if (this->blocks[vect[block]].model.getTranslation().y <= -4) {
			this->blocks[vect[block]].fallen = true;
		}
		else {
			this->blocks[vect[block]].vibrate(et);
		}
	}
}
float World::easeOutQuint(float x) {
	return 1 - pow(1 - x, 5);
}

void World::penguinCollision(Vector3 targetPos, float elapsed_time)
{
	for (int i = 0; i < penguins.size(); i++) {
		Penguin* currentPingu = &penguins[i];

		Vector3 characterTargetCenter = targetPos + Vector3(0, 1, 0);
		Vector3 coll;
		Vector3 collmore;

		if (currentPingu->mesh->testSphereCollision(currentPingu->model, characterTargetCenter, 0.5, coll, collmore)) {
			Vector3 push_away = normalize(coll - characterTargetCenter) * elapsed_time;

			if (player.dir == Player::type::LEFT) {
				currentPingu->dir = Penguin::type::LEFT;
				currentPingu->model.translate(-easeOutQuint(4 * elapsed_time), 0, 0);
				/*currentPingu->model.rotate(-90 * DEG2RAD, Vector3(0, 1, 0));*/
			}
			else if (player.dir == Player::type::RIGHT) {
				currentPingu->dir = Penguin::type::RIGHT;
				currentPingu->model.translate(easeOutQuint(4 * elapsed_time), 0, 0);
				/*currentPingu->model.rotate(90 * DEG2RAD, Vector3(0, 1, 0));*/
			}
			else if (player.dir == Player::type::FORWARD) {
				currentPingu->dir = Penguin::type::FORWARD;
				currentPingu->model.translate(0, 0, -easeOutQuint(4 * elapsed_time));
				/*currentPingu->model.rotate(0 * DEG2RAD, Vector3(0, 1, 0));*/
			}
			else if (player.dir == Player::type::BACKWARD) {
				currentPingu->dir = Penguin::type::BACKWARD;
				currentPingu->model.translate(0, 0, easeOutQuint(4 * elapsed_time));
				/*currentPingu->model.rotate(180 * DEG2RAD, Vector3(0, 1, 0));*/
			}
			targetPos = player.pos - push_away;

			targetPos.y = currentPingu->model.getTranslation().y;
		}

		/*for (int j = 0; j < penguins.size(); j++) {
			Penguin* currentPingu2 = &penguins[j];

			Vector3 penguinTargetCenter = currentPingu->pos + Vector3(0, 1, 0);
			Vector3 coll2;
			Vector3 collmore2;

			if (currentPingu2->mesh->testSphereCollision(currentPingu2->model, penguinTargetCenter, 0.5, coll2, collmore2)) {
				Vector3 push_away = normalize(coll2 - penguinTargetCenter) * elapsed_time;

				currentPingu->pos = currentPingu->pos - push_away;

				currentPingu->pos.y = currentPingu2->model.getTranslation().y;
			}
		}*/
	}
}