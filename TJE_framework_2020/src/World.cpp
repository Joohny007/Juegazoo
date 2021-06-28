#include "World.h"
#include "animation.h"

World::World(int x)
{
	this->pingu_counter = 0;
	this->max_penguins = 15;
	this->max_blocks = 56;
	this->players = { Player(0), Player(1) };
	this->player = Player(0);
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

void World::renderPenguins(bool renderBoundings, Camera* camera)
{
	for (int id = 0; id < max_blocks; id++) {
		Penguin* currentPingu = &this->penguins[id];
		currentPingu->render(camera);
		//currentPingu->pos = currentPingu->model.getTranslation();
		currentPingu->model.setTranslation(currentPingu->pos.x, currentPingu->pos.y, currentPingu->pos.z);
		currentPingu->model.rotate(180 * DEG2RAD, Vector3(0, 1, 0));
		//this->penguins[id].model.translate(this->penguins[id].pos.x, this->penguins[id].pos.y, this->penguins[id].pos.z);

		if (renderBoundings) {
			currentPingu->mesh->renderBounding(this->penguins[id].model);
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
		if (id == 23) {
			blocks[id].inicialize23();
		}
		if (id == 32) {
			blocks[id].inicialize32();
		}
		if (pos.x == (7*6.5) + 1) {
			pos.x = 1;
			pos.z += 4.9;
		}
	}
}

void World::renderBlocks(bool renderBoundings, Camera* camera)
{
	for (int id = 0; id < max_blocks; id++) {
		this->blocks[id].render(camera);
		if (renderBoundings) {
			this->blocks[id].mesh->renderBounding(this->blocks[id].model);
		}
	}
}

void World::inicializePlayers()
{
	this->players[0].inicialize();
	this->players[1].inicialize2();

}

void World::renderPlayer1(Camera* camera)
{

	this->players[0].render(camera);
	//this->players[0].model.setTranslation(10, 2, 5);
}

void World::renderPlayer2(Camera* camera)
{

	this->players[1].render(camera);
	//this->players[1].model.setTranslation(40, 2, 5);
}


void World::inicializeSky()
{
	this->sky_mesh = Mesh::Get("data/cielo.ASE");
	this->sky_tex = Texture::Get("data/cielo.tga");
	this->sky_shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
}

void World::renderSky(Camera* camera)
{
	//get the last camera that was activated
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
	sea_mesh->createSubdividedPlane(100000, 40, true);
}

void World::renderSea(Camera* camera)
{
	//get the last camera that was activated
	Matrix44 model = this->sea_model;
	

	//enable shader and pass uniforms
	//sea_mesh->createSubdividedPlane(100000, 40, true);
	//sea_model.translate(camera->eye.x * 0.01, 0.0f, camera->eye.z * 0.01);

	sea_shader->enable();
	sea_shader->setUniform("u_color", Vector4(1.0, 1.0, 1.0, 1.0));
	sea_shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
	sea_shader->setUniform("u_texture", sea_tex, 0);
	sea_shader->setUniform("u_detail_texture", Texture::Get("data/water_normalmap.tga"), 1);
	sea_shader->setUniform("u_time", Game::instance->time);
	sea_shader->setUniform("u_camera_position", camera->eye);
	sea_shader->setUniform("u_model", model);
	sea_shader->setUniform("u_texture_tiling", 1.0f);
	sea_shader->setUniform("u_fog_settings", Vector2(0.0f, 2000.0f));



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

void World::penguinCollision(Player& player, Vector3& targetPos, float elapsed_time, float time_float)
{
	for (int i = 0; i < penguins.size(); i++) {
		Penguin* currentPingu = &penguins[i];
		float speed_peng = currentPingu->speed * elapsed_time;

		Vector3& penguinFront = currentPingu->model.rotateVector(Vector3(0.0f, 0.0f, -1.0f));
		Vector3& penguinRight = currentPingu->model.rotateVector(Vector3(1.0f, 0.0f, 0.0f));

		Vector3 characterTargetCenter = targetPos + Vector3(0, 1, 0);
		Vector3 coll;
		Vector3 collmore;

		if (!player.kick) {
			if (currentPingu->mesh->testSphereCollision(currentPingu->model, characterTargetCenter, 0.5, coll, collmore)) {
				Vector3 push_away = normalize(coll - characterTargetCenter) * elapsed_time;
				
				currentPingu->penguinSpeed = Vector3(0, 0, 0);
				if (player.dir == Player::type::LEFT) {
					currentPingu->dir = Penguin::type::LEFT;
					//currentPingu->model.rotate(-90 * DEG2RAD, Vector3(0, 1, 0));
					currentPingu->model.translate(-easeOutQuint(30 * elapsed_time), 0, 0);
					currentPingu->pos = currentPingu->model.getTranslation();
					currentPingu->penguinSpeed = currentPingu->penguinSpeed + (penguinFront * -speed_peng);
				}
				else if (player.dir == Player::type::RIGHT) {
					currentPingu->dir = Penguin::type::RIGHT;
					//currentPingu->model.rotate(90 * DEG2RAD, Vector3(0, 1, 0));
					currentPingu->model.translate(easeOutQuint(30 * elapsed_time), 0, 0);
					currentPingu->pos = currentPingu->model.getTranslation();
					currentPingu->penguinSpeed = currentPingu->penguinSpeed + (penguinFront * speed_peng);
				}
				else if (player.dir == Player::type::FORWARD) {
					currentPingu->dir = Penguin::type::FORWARD;
					//currentPingu->model.rotate(0 * DEG2RAD, Vector3(0, 1, 0));
					currentPingu->model.translate(0, 0, -easeOutQuint(30 * elapsed_time));
					currentPingu->pos = currentPingu->model.getTranslation();
					currentPingu->penguinSpeed = currentPingu->penguinSpeed + (penguinRight * speed_peng);
				}
				else if (player.dir == Player::type::BACKWARD) {
					currentPingu->dir = Penguin::type::BACKWARD;
					//currentPingu->model.rotate(180 * DEG2RAD, Vector3(0, 1, 0));
					currentPingu->model.translate(0, 0, easeOutQuint(30 * elapsed_time));
					currentPingu->pos = currentPingu->model.getTranslation();
					currentPingu->penguinSpeed = currentPingu->penguinSpeed + (penguinRight * -speed_peng);
				}
				targetPos = player.pos - push_away;

				targetPos.y = currentPingu->model.getTranslation().y;
			}
		}
		else {
			if (!currentPingu->isMoving) { //no sabemos hacia donde se tiene que mover el pinguino
				if (currentPingu->mesh->testSphereCollision(currentPingu->model, characterTargetCenter, 0.8, coll, collmore)) {
					Vector3 push_away = normalize(coll - characterTargetCenter) * elapsed_time;

					currentPingu->penguinSpeed = Vector3(0, 0, 0);
					Vector3 position = currentPingu->model.getTranslation();
					if (player.dir == Player::type::LEFT) {
						currentPingu->dir = Penguin::type::LEFT;
						currentPingu->isMoving = true;
					}
					else if (player.dir == Player::type::RIGHT) {
						currentPingu->dir = Penguin::type::RIGHT;
						/*currentPingu->model.setRotation(90 * DEG2RAD, Vector3(0, 1, 0));
						currentPingu->model.translate(position.z, position.y, position.x);*/
						currentPingu->isMoving = true;
					}
					else if (player.dir == Player::type::FORWARD) {
						currentPingu->dir = Penguin::type::FORWARD;
						/*currentPingu->model.setRotation(0 * DEG2RAD, Vector3(0, 1, 0));
						currentPingu->model.translate(position.z, position.y, position.x);*/
						currentPingu->isMoving = true;
					}
					else if (player.dir == Player::type::BACKWARD) {
						currentPingu->dir = Penguin::type::BACKWARD;
						/*currentPingu->model.setRotation(180 * DEG2RAD, Vector3(0, 1, 0));
						currentPingu->model.translate(position.z, position.y, position.x);*/
						/*currentPingu->model.translate(0, 0, easeOutQuint(200 * elapsed_time));
						currentPingu->pos = currentPingu->model.getTranslation();
						currentPingu->penguinSpeed = currentPingu->penguinSpeed + (penguinRight * -speed_peng);*/
						currentPingu->isMoving = true;
					}
					targetPos = player.pos - push_away;

					targetPos.y = currentPingu->model.getTranslation().y;
				}
			}
			else { //si ya sabemos la direccion que va el pinguïno solo hace falta que siga esa direccion
				if (currentPingu->mesh->testSphereCollision(currentPingu->model, characterTargetCenter, 0.8, coll, collmore)) {
					Vector3 push_away = normalize(coll - characterTargetCenter) * elapsed_time;

					currentPingu->penguinSpeed = Vector3(0, 0, 0);
					Vector3 position = currentPingu->model.getTranslation();
					if (currentPingu->dir == Player::type::LEFT) {

						currentPingu->model.translate(-2, 0, 0);
						currentPingu->acumulative += 2;
						currentPingu->pos = currentPingu->model.getTranslation();
						currentPingu->penguinSpeed = currentPingu->penguinSpeed + (penguinFront * -speed_peng);
					}
					else if (currentPingu->dir == Player::type::RIGHT) {

						currentPingu->model.translate(2, 0, 0);
						currentPingu->acumulative += 2;
						currentPingu->pos = currentPingu->model.getTranslation();
						currentPingu->penguinSpeed = currentPingu->penguinSpeed + (penguinFront * speed_peng);
					}
					else if (currentPingu->dir == Player::type::FORWARD) {

						currentPingu->model.translate(0, 0, -2 );
						currentPingu->acumulative += 2;
						currentPingu->pos = currentPingu->model.getTranslation();
						currentPingu->penguinSpeed = currentPingu->penguinSpeed + (penguinRight * speed_peng);
					}
					else if (currentPingu->dir == Player::type::BACKWARD) {

						currentPingu->model.translate(0, 0, 2);
						currentPingu->acumulative += 2;
						currentPingu->pos = currentPingu->model.getTranslation();
						currentPingu->penguinSpeed = currentPingu->penguinSpeed + (penguinRight * -speed_peng);
					}
					targetPos = player.pos - push_away;

					targetPos.y = currentPingu->model.getTranslation().y;

					if (currentPingu->acumulative == 100.0) {
						currentPingu->isMoving = false;
						currentPingu->acumulative = 0;
					}
				}
			}
		}

		Vector3& pengTargetPos = currentPingu->pos + currentPingu->penguinSpeed;

		for (int j = i + 1; j < penguins.size(); j++) {
			Penguin* penguin2 = &penguins[j];
			float penguin_radius2 = 0.5;

			Vector3& penguin2Front = penguin2->model.rotateVector(Vector3(0.0f, 0.0f, -1.0f));
			Vector3& penguin2Right = penguin2->model.rotateVector(Vector3(1.0f, 0.0f, 0.0f));


			Vector3 penguinTargetCenter = pengTargetPos + Vector3(0, 1, 0);
			Vector3 coll2;
			Vector3 collmore2;
			if (penguin2->mesh->testSphereCollision(penguin2->model, penguinTargetCenter, 0.5, coll2, collmore2)) {
				Vector3 push_away = normalize(coll2 - penguinTargetCenter) * elapsed_time;

				pengTargetPos = currentPingu->pos - push_away;

				pengTargetPos.y = penguin2->model.getTranslation().y;
				currentPingu->pos = pengTargetPos;

			}
		}
	}
}

void World::kickAnimation1(Camera* camera, Shader* skinning, Shader* shader, Texture* textureMesh, Animation* anim, float time_float)
{
	Player& player1 = players[0];
	if (player1.kick) {
		if (time_float - player1.kick_counter <= anim->duration) {
			anim->assignTime(time_float);
			skinning->enable();

			skinning->setUniform("u_color", Vector4(1, 1, 1, 1));
			skinning->setUniform("u_viewprojection", camera->viewprojection_matrix);
			skinning->setUniform("u_texture", player1.texture, 0);
			float a = easeOutQuint(time_float);
			skinning->setUniform("u_time", time_float);
			skinning->setUniform("u_model", player1.model);
			skinning->setUniform("u_texture_tiling", 1.0f);
			player1.mesh->renderAnimated(GL_TRIANGLES, &anim->skeleton);

			skinning->disable();
		}
		else {
			if (time_float - player1.kick_counter >= player1.kick_cooldown) {
				player1.kick = false;
			}
			else {
				shader->enable();

				shader->setUniform("u_color", Vector4(1, 1, 1, 1));
				shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
				shader->setUniform("u_texture", textureMesh, 0);
				shader->setUniform("u_model", Matrix44());
				shader->setUniform("u_time", time_float);

				renderPlayer1(camera);
				shader->disable();
			}
		}
	}
	else {
		shader->enable();

		shader->setUniform("u_color", Vector4(1, 1, 1, 1));
		shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
		shader->setUniform("u_texture", textureMesh, 0);
		shader->setUniform("u_model", Matrix44());
		shader->setUniform("u_time", time_float);

		renderPlayer1(camera);
		shader->disable();
	}
	//-----------------------------------------------------------------------------------------------------------------------------------------------
	Player& player2 = players[1];
	if (player2.kick) {
		if (time_float - player2.kick_counter <= anim->duration) {
			anim->assignTime(time_float);
			skinning->enable();

			skinning->setUniform("u_color", Vector4(1, 1, 1, 1));
			skinning->setUniform("u_viewprojection", camera->viewprojection_matrix);
			skinning->setUniform("u_texture", player2.texture, 0);
			float a = easeOutQuint(time_float);
			skinning->setUniform("u_time", time_float);
			skinning->setUniform("u_model", player2.model);
			skinning->setUniform("u_texture_tiling", 1.0f);
			player2.mesh->renderAnimated(GL_TRIANGLES, &anim->skeleton);

			skinning->disable();
		}
		else {
			if (time_float - player2.kick_counter >= player.kick_cooldown) {
				player2.kick = false;
			}
			else {
				shader->enable();

				shader->setUniform("u_color", Vector4(1, 1, 1, 1));
				shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
				shader->setUniform("u_texture", textureMesh, 0);
				shader->setUniform("u_model", Matrix44());
				shader->setUniform("u_time", time_float);

				renderPlayer2(camera);
				shader->disable();
			}
		}
	}
	else {
		shader->enable();

		shader->setUniform("u_color", Vector4(1, 1, 1, 1));
		shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
		shader->setUniform("u_texture", textureMesh, 0);
		shader->setUniform("u_model", Matrix44());
		shader->setUniform("u_time", time_float);

		renderPlayer2(camera);
		shader->disable();
	}
}

void World::kickAnimation2(Camera* player2Cam, Shader* skinning, Shader* shader, Texture* textureMesh, Animation* anim, float time_float)
{
	Player& player2 = players[1];
	if (player2.kick) {
		if (time_float - player2.kick_counter <= anim->duration) {
			anim->assignTime(time_float);
			skinning->enable();

			skinning->setUniform("u_color", Vector4(1, 1, 1, 1));
			skinning->setUniform("u_viewprojection", player2Cam->viewprojection_matrix);
			skinning->setUniform("u_texture", player2.texture, 0);
			float a = easeOutQuint(time_float);
			skinning->setUniform("u_time", time_float);
			skinning->setUniform("u_model", player2.model);
			skinning->setUniform("u_texture_tiling", 1.0f);
			player2.mesh->renderAnimated(GL_TRIANGLES, &anim->skeleton);

			skinning->disable();
		}
		else {
			if (time_float - player2.kick_counter >= player2.kick_cooldown) {
				player2.kick = false;
			}
			else {
				shader->enable();

				shader->setUniform("u_color", Vector4(1, 1, 1, 1));
				shader->setUniform("u_viewprojection", player2Cam->viewprojection_matrix);
				shader->setUniform("u_texture", textureMesh, 0);
				shader->setUniform("u_model", Matrix44());
				shader->setUniform("u_time", time_float);

				renderPlayer2(player2Cam);
				shader->disable();
			}
		}
	}
	else {
		shader->enable();

		shader->setUniform("u_color", Vector4(1, 1, 1, 1));
		shader->setUniform("u_viewprojection", player2Cam->viewprojection_matrix);
		shader->setUniform("u_texture", textureMesh, 0);
		shader->setUniform("u_model", Matrix44());
		shader->setUniform("u_time", time_float);

		renderPlayer2(player2Cam);
		shader->disable();
	}
	//---------------------------------------------------------------------------------------------------------------------------------------------
	Player& player1 = players[0];
	if (player1.kick) {
		if (time_float - player1.kick_counter <= anim->duration) {
			anim->assignTime(time_float);
			skinning->enable();

			skinning->setUniform("u_color", Vector4(1, 1, 1, 1));
			skinning->setUniform("u_viewprojection", player2Cam->viewprojection_matrix);
			skinning->setUniform("u_texture", player1.texture, 0);
			float a = easeOutQuint(time_float);
			skinning->setUniform("u_time", time_float);
			skinning->setUniform("u_model", player1.model);
			skinning->setUniform("u_texture_tiling", 1.0f);
			player1.mesh->renderAnimated(GL_TRIANGLES, &anim->skeleton);

			skinning->disable();
		}
		else {
			if (time_float - player1.kick_counter >= player1.kick_cooldown) {
				player1.kick = false;
			}
			else {
				shader->enable();

				shader->setUniform("u_color", Vector4(1, 1, 1, 1));
				shader->setUniform("u_viewprojection", player2Cam->viewprojection_matrix);
				shader->setUniform("u_texture", textureMesh, 0);
				shader->setUniform("u_model", Matrix44());
				shader->setUniform("u_time", time_float);

				renderPlayer1(player2Cam);
				shader->disable();
			}
		}
	}
	else {
		shader->enable();

		shader->setUniform("u_color", Vector4(1, 1, 1, 1));
		shader->setUniform("u_viewprojection", player2Cam->viewprojection_matrix);
		shader->setUniform("u_texture", textureMesh, 0);
		shader->setUniform("u_model", Matrix44());
		shader->setUniform("u_time", time_float);

		renderPlayer1(player2Cam);
		shader->disable();
	}

}

void World::moving1(Camera* camera, Shader* skinning, Shader* shader, Texture* textureMesh, Animation* anim, float time_float)
{
	Player& player1 = players[0];
	if (player1.moving) {
		if (time_float - player1.moving_counter <= anim->duration-0.5) {
			anim->assignTime(time_float);
			skinning->enable();

			skinning->setUniform("u_color", Vector4(1, 1, 1, 1));

			skinning->setUniform("u_viewprojection", camera->viewprojection_matrix);
			skinning->setUniform("u_texture", player1.texture, 0);
			float a = easeOutQuint(20);
			skinning->setUniform("u_time", time_float);
			skinning->setUniform("u_model", player1.model);
			skinning->setUniform("u_texture_tiling", 1.0f);
			player1.mesh->renderAnimated(GL_TRIANGLES, &anim->skeleton);

			skinning->disable();
		}
		else {
			player1.moving = false;
			player1.pos.z += 1.5;
		}
	}
	else {
		shader->enable();

		shader->setUniform("u_color", Vector4(1, 1, 1, 1));
		shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
		shader->setUniform("u_texture", textureMesh, 0);
		shader->setUniform("u_model", Matrix44());
		shader->setUniform("u_time", time_float);

		renderPlayer1(camera);
		shader->disable();
	}
	//-------------------------------------------------------------------------------------------------------
	Player& player2 = players[1];
	if (player2.moving) {
		if (time_float - player2.moving_counter <= anim->duration - 0.5) {
			anim->assignTime(time_float);
			skinning->enable();

			skinning->setUniform("u_color", Vector4(1, 1, 1, 1));

			skinning->setUniform("u_viewprojection", camera->viewprojection_matrix);
			skinning->setUniform("u_texture", player2.texture, 0);
			float a = easeOutQuint(20);
			skinning->setUniform("u_time", time_float);
			skinning->setUniform("u_model", player2.model);
			skinning->setUniform("u_texture_tiling", 1.0f);
			player2.mesh->renderAnimated(GL_TRIANGLES, &anim->skeleton);

			skinning->disable();
		}
		else { 
			player2.moving = false;
			player2.pos.z += 1.5;
		}
	}
	else {
		shader->enable();

		shader->setUniform("u_color", Vector4(1, 1, 1, 1));
		shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
		shader->setUniform("u_texture", textureMesh, 0);
		shader->setUniform("u_model", Matrix44());
		shader->setUniform("u_time", time_float);

		renderPlayer2(camera);
		shader->disable();
	}
}

void World::moving2(Camera* player2Cam, Shader* skinning, Shader* shader, Texture* textureMesh, Animation* anim, float time_float)
{
	Player& player2 = players[1];
	if (player2.moving) {
		if (time_float - player2.moving_counter <= anim->duration - 0.5) {
			anim->assignTime(time_float);
			skinning->enable();

			skinning->setUniform("u_color", Vector4(1, 1, 1, 1));

			skinning->setUniform("u_viewprojection", player2Cam->viewprojection_matrix);
			skinning->setUniform("u_texture", player2.texture, 0);
			float a = easeOutQuint(20);
			skinning->setUniform("u_time", time_float);
			skinning->setUniform("u_model", player2.model);
			skinning->setUniform("u_texture_tiling", 1.0f);
			player2.mesh->renderAnimated(GL_TRIANGLES, &anim->skeleton);

			skinning->disable();
		}
		else { 
			player2.moving = false;
			player2.pos.z += 1.5;
		}
	}
	else {
		shader->enable();

		shader->setUniform("u_color", Vector4(1, 1, 1, 1));
		shader->setUniform("u_viewprojection", player2Cam->viewprojection_matrix);
		shader->setUniform("u_texture", textureMesh, 0);
		shader->setUniform("u_model", Matrix44());
		shader->setUniform("u_time", time_float);

		renderPlayer2(player2Cam);
		shader->disable();
	}
	//-------------------------------------------------------------------------------------------------------------------------
	Player& player1 = players[0];
	if (player1.moving) {
		if (time_float - player1.moving_counter <= anim->duration - 0.5) {
			anim->assignTime(time_float);
			skinning->enable();

			skinning->setUniform("u_color", Vector4(1, 1, 1, 1));

			skinning->setUniform("u_viewprojection", player2Cam->viewprojection_matrix);
			skinning->setUniform("u_texture", player1.texture, 0);
			float a = easeOutQuint(20);
			skinning->setUniform("u_time", time_float);
			skinning->setUniform("u_model", player1.model);
			skinning->setUniform("u_texture_tiling", 1.0f);
			player1.mesh->renderAnimated(GL_TRIANGLES, &anim->skeleton);

			skinning->disable();
		}
		else { 
			player1.moving = false;
			player1.pos.z += 1.5;
		}
	}
	else {
		shader->enable();

		shader->setUniform("u_color", Vector4(1, 1, 1, 1));
		shader->setUniform("u_viewprojection", player2Cam->viewprojection_matrix);
		shader->setUniform("u_texture", textureMesh, 0);
		shader->setUniform("u_model", Matrix44());
		shader->setUniform("u_time", time_float);

		renderPlayer1(player2Cam);
		shader->disable();
	}
}

void World::stunPlayer1(Vector3& player2_targetPos, float time)
{
	Player& player1 = players[0];
	Player& player2 = players[1];
	if (player1.stunned) {
		if (time - player1.stun_counter >= 3.0f) {
			player1.stunned = false;
		}
	}
	else if (player2.kick) {
		Vector3 characterTargetCenter = player2_targetPos + Vector3(0, 1, 0);
		Vector3 coll;
		Vector3 collmore;

		if (player1.mesh->testSphereCollision(player1.model, characterTargetCenter, 1.2, coll, collmore)) {
			player1.stunned = true;
			player1.stun_counter = time;
		}
	}
}

void World::stunPlayer2(Vector3& player1_targetPos, float time)
{
	Player& player1 = players[0];
	Player& player2 = players[1];
	if (player2.stunned) {
		if (time - player2.stun_counter >= 3.0f) {
			player2.stunned = false;
		}
	}
	else if (player1.kick) {
		Vector3 characterTargetCenter = player1_targetPos + Vector3(0, 1, 0);
		Vector3 coll;
		Vector3 collmore;

		if (player2.mesh->testSphereCollision(player2.model, characterTargetCenter, 1.2, coll, collmore)) {
			player2.stunned = true;
			player2.stun_counter = time;
		}
	}
}

void World::renderGUI(Camera* cam)
{
	
}

void World::playSound(const char* sound)
{
	if (BASS_Init(-1, 44100, 0, 0, NULL) == false) //-1 significa usar el por defecto del sistema operativo
	{
		//error abriendo la tarjeta de sonido...
	}
	HSAMPLE hSample;
	HCHANNEL hSampleChannel;
	hSample = BASS_SampleLoad(false, sound, 0, 0, 3, 0);
	if (hSample == 0)
	{
	}

	hSampleChannel = BASS_SampleGetChannel(hSample, false);


	//Lanzamos un sample
	BASS_ChannelPlay(hSampleChannel, true);
}