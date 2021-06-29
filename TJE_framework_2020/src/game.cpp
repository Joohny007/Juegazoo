#include "game.h"
#include "utils.h"
#include "mesh.h"
#include "texture.h"
#include "fbo.h"
#include "shader.h"
#include "input.h"
#include "animation.h"
#include "Player.h"
#include "World.h"
#include <iostream>
#include <cmath>
#include <chrono>
#include <ctime>
#include "extra/bass.h"

//some globals JOAN ERES UNA BELLISIMA PERSONA ESPERO QUE TE VAYA TODO BIEN EN LA VIDA PRECIOSO QUE GRANDE OJALA REGALRTE UN DURUM
Mesh* mesh = NULL;
Mesh* mesh2 = NULL;
Mesh* mesh3 = NULL;
Mesh* grass = NULL;
Texture* texture = NULL;
Texture* textureMesh = NULL;
Shader* shader = NULL;
Shader* skinning = NULL;
Animation* anim = NULL;
Animation* anim_woman = NULL;
Animation* walk_anim = NULL;
Animation* walk_anim_woman = NULL;
float counter = 0.0f;
float angle = 0;
float mouse_speed = 10.0f;

enum stages {
	TITLE,
	HOW_TO_PLAY,
	TUTORIAL,
	GAME,
	END
};
stages st = TITLE;

std::vector<int> bloques;
FBO* fbo = NULL;

bool free_camera = false;
World world(2);
Player& player1 = world.players[0];
Player& player2 = world.players[1];
bool renderBoundings;
Game* Game::instance = NULL;

Game::Game(int window_width, int window_height, SDL_Window* window)
{
	this->window_width = window_width;
	this->window_height = window_height;
	this->window = window;
	instance = this;
	must_exit = false;

	fps = 0;
	frame = 0;
	time = 0.0f;
	elapsed_time = 0.0f;
	mouse_locked = false;

	//EntityMesh player;

	//OpenGL flags
	glEnable( GL_CULL_FACE ); //render both sides of every triangle
	glEnable( GL_DEPTH_TEST ); //check the occlusions using the Z buffer

	//create our camera
	camera = new Camera();
	camera->lookAt(Vector3(0.f,100.f, 100.f),Vector3(0.f,0.f,0.f), Vector3(0.f,1.f,0.f)); //position the camera and point to 0,0,0
	camera->setPerspective(70.f,window_width/(float)window_height,0.1f,10000.f); //set the projection, we want to be perspective

	player2Cam = new Camera();
	player2Cam->lookAt(Vector3(0.f, 100.f, 100.f), Vector3(0.f, 0.f, 0.f), Vector3(0.f, 1.f, 0.f)); //position the camera and point to 0,0,0
	player2Cam->setPerspective(70.f, window_width / (float)window_height, 0.1f, 10000.f); //set the projection, we want to be perspective

	TitleCam = new Camera();
	TitleCam->lookAt(Vector3(70.f, 100.f, 100.f), Vector3(0.f, 0.f, 0.f), Vector3(0.f, 1.f, 0.f)); //position the camera and point to 0,0,0
	TitleCam->setPerspective(70.f, window_width / (float)window_height, 0.1f, 10000.f); //set the projection, we want to be perspective

	world.inicializeSky();
	world.inicializeSea();
	world.inicializeBlocks();
	world.inicializePenguins();
	world.inicializePlayers();
	renderBoundings = true;

	//grass = Mesh::Get("data/GiantGeneralPack/Grass_T/grass-long_orange_8.obj");
	textureMesh = Texture::Get("data/initialShadingGroup_Base_Color3.png");

	// example of shader loading using the shaders manager
	shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
	skinning = Shader::Get("data/shaders/skinning.vs", "data/shaders/texture.fs");

	//hide the cursor
	SDL_ShowCursor(!mouse_locked); //hide or show the mouse
	auto start_time = std::chrono::system_clock::now();
}

void kickGUI(int x1, int x2, int x3, int x4, const char* s, bool player_kick) {
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	Camera cam2D;
	cam2D.setOrthographic(0, Game::instance->window_width, Game::instance->window_height, 0, -1, 1);
	cam2D.enable();
	Mesh quad;
	quad.createQuad(x1, x2, x3, x4, true);
	Shader* shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
	shader->enable();
	Vector4 normalColor = Vector4(1, 1, 1, 1);
	Vector4 gris = Vector4(1, 1, 1, 0.35);
	shader->setUniform("u_color", player_kick ? gris : normalColor);
	shader->setUniform("u_viewprojection", cam2D.viewprojection_matrix);
	shader->setUniform("u_texture", Texture::Get(s), 0);
	Matrix44 quadModel;
	shader->setUniform("u_model", quadModel);
	shader->setUniform("u_texture_tilling", 1.0f);
	quad.render(GL_TRIANGLES);
	shader->disable();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glDisable(GL_BLEND);
}

void GUI(int x1, int x2, int x3, int x4, const char* s) {
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	Camera cam2D;
	cam2D.setOrthographic(0, Game::instance->window_width, Game::instance->window_height, 0, -1, 1);
	cam2D.enable();
	Mesh quad;
	quad.createQuad(x1, x2, x3, x4, true);
	Shader* shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
	shader->enable();
	shader->setUniform("u_color", Vector4(1, 1, 1, 1));
	shader->setUniform("u_viewprojection", cam2D.viewprojection_matrix);
	shader->setUniform("u_texture", Texture::Get(s), 0);
	Matrix44 quadModel;
	shader->setUniform("u_model", quadModel);
	shader->setUniform("u_texture_tilling", 1.0f);
	quad.render(GL_TRIANGLES);
	shader->disable();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glDisable(GL_BLEND);
}

void RenderFirstCam(Camera* camera, float time_float)
{
	glViewport(0, 0, Game::instance->window_width / 2, Game::instance->window_height);
	
	world.renderSky(camera);
	world.renderSea(camera);


	if (!free_camera) {
		player1.model.setTranslation(player1.pos.x, player1.pos.y, player1.pos.z);
		player2.model.setTranslation(player2.pos.x, player2.pos.y, player2.pos.z);

		if (st == END) {
			Vector3 eye2 = player1.model * Vector3(0.0f, 2.0f, 3.5f);
			Vector3 center2 = player1.model * Vector3(0.0f, 0.0f, -10.0f);
			Vector3 up = Vector3(0.0f, 1.0f, 0.0f);
			camera->lookAt(eye2, center2, up);
		}
		else {
			Vector3 eye1 = player1.model * Vector3(0.0f, 8.0f, -5.5f);
			Vector3 center1 = player1.model * Vector3(0.0f, 0.0f, 10.0f);
			Vector3 up = Vector3(0.0f, 1.0f, 0.0f);
			camera->lookAt(eye1, center1, up);
		}
	}
	else {
		drawText((Game::instance->window_width / 2) - 200, Game::instance->window_height / 2 - 50, "PAUSE", Vector3(0, 0.098, 0.098), 13);
	}

	Animation* anim_woman = Animation::Get("data/Animaciones/Skanim/mma_kick_woman.skanim");

	if (st == GAME) {
		world.kickAnimation1(camera, skinning, shader, textureMesh, anim_woman, time_float);
	}
	world.renderBlocks(renderBoundings, camera);
	world.renderPenguins(renderBoundings, camera);
	float x = Game::instance->window_width - (Game::instance->window_width - 150);
	float y = Game::instance->window_height - (Game::instance->window_height - 60);
	kickGUI(x, y, x, y, "data/GUI/high-kick.png", player1.kick);

	float x2 = Game::instance->window_width - 80;
	float y2 = Game::instance->window_height - 50;
	GUI( x2, y2, 120 , 70, "data/GUI/medallist.png");

	if (player1.stunned) {
		x = Game::instance->window_width / 2;
		y = Game::instance->window_height - (Game::instance->window_height - 190);
		GUI(x, y, 520, 190, "data/GUI/stunned.png");
	}

	drawText(x2 - 300, 20, "SCORE =" + player1.scoreToString(), Vector3(0, 0, 1), 6);


	if (st == END) {
		Animation* victory = Animation::Get("data/Animaciones/Skanim/victory.skanim");
		Animation* victory_woman = Animation::Get("data/Animaciones/Skanim/victory_woman.skanim");
		Animation* defeat = Animation::Get("data/Animaciones/Skanim/defeated.skanim");
		Animation* defeat_woman = Animation::Get("data/Animaciones/Skanim/defeated_woman.skanim");
		Animation* draw_man = Animation::Get("data/Animaciones/Skanim/belly_dance.skanim");
		Animation* draw_woman = Animation::Get("data/Animaciones/Skanim/belly_dance_woman.skanim");

		if (player1.score > player2.score) {
			float x2 = Game::instance->window_width - 400;
			float y2 = Game::instance->window_height - 300;
			world.player1Victory(camera, skinning, shader, textureMesh, victory_woman, defeat, time_float);
			GUI(x2, y2, 800, 500, "data/GUI/YOUWIN.png");
		}
		else if (player1.score < player2.score) {
			float x2 = Game::instance->window_width - 400;
			float y2 = Game::instance->window_height - 300;
			world.player2Victory(camera, skinning, shader, textureMesh, victory, defeat_woman, time_float);
			GUI(x2, y2, 800, 500, "data/GUI/YOULOSE.png");
		}
		else {
			float x2 = Game::instance->window_width - 400;
			float y2 = Game::instance->window_height - 300;
			world.DRAW1(camera, skinning, shader, textureMesh, draw_woman, draw_man, time_float);
			GUI(x2, y2, 800, 500, "data/GUI/DRAW.png");
		}
	}
}

void RenderSecondCam(Camera* player2Cam, float time_float)
{
	glViewport(Game::instance->window_width / 2, 0, Game::instance->window_width / 2, Game::instance->window_height);

	world.renderSky(player2Cam);
	world.renderSea(player2Cam);


	if (!free_camera) {
		player1.model.setTranslation(player1.pos.x, player1.pos.y, player1.pos.z);
		player2.model.setTranslation(player2.pos.x, player2.pos.y, player2.pos.z);

		if (st == END) {
			Vector3 eye2 = player2.model * Vector3(0.0f, 2.0f, 3.5f);
			Vector3 center2 = player2.model * Vector3(0.0f, 0.0f, -10.0f);
			Vector3 up = Vector3(0.0f, 1.0f, 0.0f);
			player2Cam->lookAt(eye2, center2, up);
		}
		else{
			Vector3 eye2 = player2.model * Vector3(0.0f, 8.0f, -5.5f);
			Vector3 center2 = player2.model * Vector3(0.0f, 0.0f, 10.0f);
			Vector3 up = Vector3(0.0f, 1.0f, 0.0f);
			player2Cam->lookAt(eye2, center2, up);
		}
	}
	else {
		drawText((Game::instance->window_width / 2) - 200, Game::instance->window_height / 2 - 50, "PAUSE", Vector3(0, 0.098, 0.098), 13);
	}

	Animation* anim = Animation::Get("data/Animaciones/Skanim/mma_kick.skanim");

	if (st == GAME) {
		world.kickAnimation2(player2Cam, skinning, shader, textureMesh, anim, time_float);
	}

	world.renderBlocks(renderBoundings, player2Cam);
	world.renderPenguins(renderBoundings, player2Cam);

	float x = Game::instance->window_width/2 - (Game::instance->window_width/2 - 150);
	float y = Game::instance->window_height - (Game::instance->window_height - 60);
	kickGUI(x, y, x, y, "data/GUI/high-kick.png", player2.kick);

	float x2 = Game::instance->window_width - 80;
	float y2 = Game::instance->window_height - 50;
	GUI(x2, y2, 120, 70, "data/GUI/medallist2.png");

	if (player2.stunned) {
		x = Game::instance->window_width / 2 ;
		y = Game::instance->window_height - (Game::instance->window_height - 190);
		GUI(x, y, 520, 190, "data/GUI/stunned.png");
	}
	drawText(x2 - 300, 20, "SCORE =" + player2.scoreToString(), Vector3(1, 0, 0), 6);

	if (st == END) {
		Animation* victory = Animation::Get("data/Animaciones/Skanim/victory.skanim");
		Animation* victory_woman = Animation::Get("data/Animaciones/Skanim/victory_woman.skanim");
		Animation* defeat = Animation::Get("data/Animaciones/Skanim/defeated.skanim");
		Animation* defeat_woman = Animation::Get("data/Animaciones/Skanim/defeated_woman.skanim");
		Animation* draw_man = Animation::Get("data/Animaciones/Skanim/belly_dance.skanim");
		Animation* draw_woman = Animation::Get("data/Animaciones/Skanim/belly_dance_woman.skanim");

		if (player2.score > player1.score) {
			float x2 = Game::instance->window_width - 400;
			float y2 = Game::instance->window_height - 300;
			world.player2Victory(player2Cam, skinning, shader, textureMesh, victory, defeat_woman, time_float);
			GUI(x2, y2, 800, 500, "data/GUI/YOUWIN.png");
		}
		else if (player2.score < player1.score) {
			float x2 = Game::instance->window_width - 400;
			float y2 = Game::instance->window_height - 300;
			world.player1Victory(player2Cam, skinning, shader, textureMesh, victory_woman, defeat, time_float);
			GUI(x2, y2, 800, 500, "data/GUI/YOULOSE.png");
		}
		else {
			float x2 = Game::instance->window_width - 400;
			float y2 = Game::instance->window_height - 300;
			world.DRAW2(player2Cam, skinning, shader, textureMesh, draw_man, draw_woman, time_float);
			GUI(x2, y2, 800, 500, "data/GUI/DRAW.png");
		}
	}

}

void RenderTitle(Camera* camera, float time_float, const char* s)
{
	glViewport(0, 0, Game::instance->window_width, Game::instance->window_height);

	world.renderSky(camera);
	world.renderSea(camera);

	shader->enable();

	shader->setUniform("u_color", Vector4(1, 1, 1, 1));
	shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
	shader->setUniform("u_texture", textureMesh, 0);
	shader->setUniform("u_model", Matrix44());
	shader->setUniform("u_time", time_float);
	shader->disable();

	world.renderBlocks(renderBoundings, camera);
	world.renderPenguins(renderBoundings, camera);

	int x1 = Game::instance->window_width / 2;
	int x2 = Game::instance->window_height / 2;
	int x3 = Game::instance->window_width;
	int x4 = Game::instance->window_height;
	GUI(x1, x2, x3, x4, s);
}

//what to do when the image has to be draw
void Game::render(void)
{
	//set the clear color (the background color)
	glClearColor(53.0, 81.0, 92.0, 1.0);

	// Clear the window and the depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//set the camera as default
	camera->enable();
	player2Cam->enable();
	TitleCam->enable();
   

	if (!shader) return;

	if (st == TITLE) { RenderTitle(TitleCam, time, "data/Vikinguinos.png"); }
	else if (st == HOW_TO_PLAY) { RenderTitle(TitleCam, time, "data/GUI/how_to_play.png"); }
	else if (st == TUTORIAL) { RenderTitle(TitleCam, time, "data/GUI/controls.png"); }
	else {
		RenderFirstCam(camera, time);
		RenderSecondCam(player2Cam, time);
	}

	//Draw the floor grid
	drawGrid();

	//render the FPS, Draw Calls, etc

	//swap between front buffer and back buffer
	SDL_GL_SwapWindow(this->window);
}

void Game::update(double seconds_elapsed)
{
	if (st == TITLE) {
		if (Input::wasKeyPressed(SDL_SCANCODE_W) || Input::wasKeyPressed(SDL_SCANCODE_UP)) { st = HOW_TO_PLAY; };

		TitleCam->rotate(0.005f, Vector3(0.0f, -1.0f, 0.0f));
	}
	else if (st == HOW_TO_PLAY) {
		if (Input::wasKeyPressed(SDL_SCANCODE_W) || Input::wasKeyPressed(SDL_SCANCODE_UP)) { st = TUTORIAL; };
	}
	else if (st == TUTORIAL) {
		if (Input::wasKeyPressed(SDL_SCANCODE_W) || Input::wasKeyPressed(SDL_SCANCODE_UP)) { st = GAME; };
	}
	else if (st == END) {
		if (Input::wasKeyPressed(SDL_SCANCODE_W) || Input::wasKeyPressed(SDL_SCANCODE_UP)) { world.replay(); st = GAME; };
	}
	else if (st == GAME) {
		//example
		angle += (float)seconds_elapsed * 10.0f;
		if (world.game_time > 10) { st = END; }
		//mouse input to rotate the cam
		if ((Input::mouse_state & SDL_BUTTON_LEFT) || mouse_locked) //is left button pressed?
		{
			camera->rotate(Input::mouse_delta.x * 0.005f, Vector3(0.0f, -1.0f, 0.0f));
			camera->rotate(Input::mouse_delta.y * 0.005f, camera->getLocalVector(Vector3(-1.0f, 0.0f, 0.0f)));
		}

		if (free_camera) {
			float speed = seconds_elapsed * mouse_speed; //the speed is defined by the seconds_elapsed so it goes constant
			//async input to move the camera around
			if (Input::isKeyPressed(SDL_SCANCODE_LSHIFT)) speed *= 10; //move faster with left shift
			if (Input::isKeyPressed(SDL_SCANCODE_W) || Input::isKeyPressed(SDL_SCANCODE_UP)) camera->move(Vector3(0.0f, 0.0f, 1.0f) * speed);
			if (Input::isKeyPressed(SDL_SCANCODE_S) || Input::isKeyPressed(SDL_SCANCODE_DOWN)) camera->move(Vector3(0.0f, 0.0f, -1.0f) * speed);
			if (Input::isKeyPressed(SDL_SCANCODE_A) || Input::isKeyPressed(SDL_SCANCODE_LEFT)) camera->move(Vector3(1.0f, 0.0f, 0.0f) * speed);
			if (Input::isKeyPressed(SDL_SCANCODE_D) || Input::isKeyPressed(SDL_SCANCODE_RIGHT)) camera->move(Vector3(-1.0f, 0.0f, 0.0f) * speed);
		}
		else {
			world.calculateScore();

			float speed1 = player1.speed * elapsed_time;
			float rot_speed1 = player1.rot_speed * elapsed_time;
			float character_radius1 = 0.5;

			//player.model.rotate(player.yaw * DEG2RAD, Vector3(0, 1, 0));

			Vector3& player1Front = player1.model.rotateVector(Vector3(0.0f, 0.0f, -1.0f));
			Vector3& player1Right = player1.model.rotateVector(Vector3(1.0f, 0.0f, 0.0f));

			//Vector3 playerSpeed;
			if (!player1.stunned) {
				player1.playerSpeed = Vector3(0, 0, 0);
				if (Input::isKeyPressed(SDL_SCANCODE_LSHIFT)) speed1 *= 2; //move faster with left shift
				if (Input::isKeyPressed(SDL_SCANCODE_W)) {
					player1.playerSpeed = player1.playerSpeed + (player1Front * -speed1);
					player1.dir = Player::type::FORWARD;
					player1.moving = true;
					player1.moving_counter = time;
					//world.player.model.rotate(180 * DEG2RAD, Vector3(0, 1, 0));
				}
				if (Input::isKeyPressed(SDL_SCANCODE_S)) {
					player1.playerSpeed = player1.playerSpeed + (player1Front * speed1);
					player1.dir = Player::type::BACKWARD;
					player1.moving = true;
					player1.moving_counter = time;
					/*world.player.model.rotate(0 * DEG2RAD, Vector3(0, 1, 0));*/
				}
				if (Input::isKeyPressed(SDL_SCANCODE_A)) {
					player1.playerSpeed = player1.playerSpeed + (player1Right * speed1);
					player1.dir = Player::type::LEFT;
					player1.moving = true;
					player1.moving_counter = time;
					//world.player.model.rotate(90 * DEG2RAD, Vector3(0, 1, 0));
				}
				if (Input::isKeyPressed(SDL_SCANCODE_D)) {
					player1.playerSpeed = player1.playerSpeed + (player1Right * -speed1);
					player1.dir = Player::type::RIGHT;
					player1.moving = true;
					player1.moving_counter = time;
					/*world.player.model.rotate(-90 * DEG2RAD, Vector3(0, 1, 0));*/
				}
				if (time - player1.kick_counter >= player1.kick_cooldown) {
					if (Input::wasKeyPressed(SDL_SCANCODE_F)) {
						player1.kick = true;
						player1.kick_counter = time;
						player1.moving = false;
					}
				}

				if (Input::isKeyPressed(SDL_SCANCODE_Q)) player1.yaw -= rot_speed1;
				if (Input::isKeyPressed(SDL_SCANCODE_E)) player1.yaw += rot_speed1;

			}
			//---------------------------------------------------------------------------------------------------------
			float speed2 = player2.speed * elapsed_time;
			float rot_speed2 = player2.rot_speed * elapsed_time;
			float character_radius2 = 0.5;

			Vector3& player2Front = player2.model.rotateVector(Vector3(0.0f, 0.0f, -1.0f));
			Vector3& player2Right = player2.model.rotateVector(Vector3(1.0f, 0.0f, 0.0f));

			if (!player2.stunned) {
				player2.playerSpeed = Vector3(0, 0, 0);
				if (Input::isKeyPressed(SDL_SCANCODE_LSHIFT)) speed2 *= 2; //move faster with left shift
				if (Input::isKeyPressed(SDL_SCANCODE_UP)) {
					player2.playerSpeed = player2.playerSpeed + (player2Front * -speed2);
					player2.dir = Player::type::FORWARD;
					player2.moving = true;
					player2.moving_counter = time;
					/*world.player.model.rotate(180 * DEG2RAD, Vector3(0, 1, 0));*/
				}
				if (Input::isKeyPressed(SDL_SCANCODE_DOWN)) {
					player2.playerSpeed = player2.playerSpeed + (player2Front * speed2);
					player2.dir = Player::type::BACKWARD;
					player2.moving = true;
					player2.moving_counter = time;
					/*world.player.model.rotate(0 * DEG2RAD, Vector3(0, 1, 0));*/
				}
				if (Input::isKeyPressed(SDL_SCANCODE_LEFT)) {
					player2.playerSpeed = player2.playerSpeed + (player2Right * speed2);
					player2.dir = Player::type::LEFT;
					player2.moving = true;
					player2.moving_counter = time;
					/*world.player.model.rotate(90 * DEG2RAD, Vector3(0, 1, 0));*/
				}
				if (Input::isKeyPressed(SDL_SCANCODE_RIGHT)) {
					player2.playerSpeed = player2.playerSpeed + (player2Right * -speed2);
					player2.dir = Player::type::RIGHT;
					player2.moving = true;
					player2.moving_counter = time;
					/*world.player.model.rotate(-90 * DEG2RAD, Vector3(0, 1, 0));*/
				}
				if (time - player2.kick_counter >= player2.kick_cooldown) {
					if (Input::wasKeyPressed(SDL_SCANCODE_L)) {
						player2.kick = true;
						player2.kick_counter = time;
						player2.moving = false;
					}
				}

				if (Input::isKeyPressed(SDL_SCANCODE_I)) player2.yaw -= rot_speed2;
				if (Input::isKeyPressed(SDL_SCANCODE_O)) player2.yaw += rot_speed2;
			}

			for (int i = 0; i < world.players.size(); i++) {
				Player& player = world.players[i];
				Vector3& playerSpeed = world.players[i].playerSpeed;
				Vector3& targetPos = world.players[i].pos + playerSpeed;

				world.penguinCollision(player, targetPos, elapsed_time, time);

				if (player.id == world.players[1].id) {
					world.stunPlayer1(targetPos, time);
				}
				else { world.stunPlayer2(targetPos, time); }

				float checker = world.isPlayeronaBlock(targetPos);
				if (checker == -5) {
					targetPos.y -= player.speed * elapsed_time;
					player.pos = targetPos;
				}
				else if (checker + 2 > player.pos.y) {
					if (checker + 2 - player.pos.y > 1) {
						float checker2 = world.isPlayeronaBlock(player.pos);
						if (checker2 - player.pos.y > 2) {
							player.pos.y -= player.speed * elapsed_time;
						}
						else {
							player.pos.y = checker2 + 2;
						}

					}
					else {
						targetPos.y = checker + 2;
						player.pos = targetPos;
					}
				}
				else {
					if (player.pos.y - player.speed * elapsed_time > checker + 2) {
						targetPos.y -= player.speed * elapsed_time;
						player.pos = targetPos;
					}
					else {
						targetPos.y = checker + 2;
						player.pos = targetPos;
					}
				}
			}
			world.BlockVibration(elapsed_time);
			for (int i = 0; i < world.penguins.size(); i++) {
				Penguin& penguin = world.penguins[i];
				if (penguin.fallen) {
					continue;
				}
				Vector3 trans = penguin.pos;
				float checker = world.isPlayeronaBlock(penguin.pos);
				if (checker == -5) {
					if (trans.y <= -2) {
						penguin.fallen = true;
						world.playSound("data/wav/Quack.wav");
					}
					penguin.pos.y -= penguin.speed * elapsed_time;
				}
				else {
					penguin.pos.y = checker + 2;

				}
			}
			if (player1.pos.y <= -4) {
				for (int i = 0; i < world.blocks.size(); i++) {
					if (world.blocks[i].fallen == false) {
						player1.pos = world.blocks[i].model.getTranslation();
						player1.score--;
						break;
					}
				}
			}
			if (player2.pos.y <= -4) {
				for (int i = 0; i < world.blocks.size(); i++) {
					if (world.blocks[i].fallen == false) {
						player2.pos = world.blocks[i].model.getTranslation();
						player2.score--;
						break;
					}
				}
			}

		}
	}


	if (Input::wasKeyPressed(SDL_SCANCODE_TAB)) {
		free_camera = !free_camera;
	}

	//to navigate with the mouse fixed in the middle
	if (mouse_locked)
		Input::centerMouse();
}

//Keyboard event handler (sync input)
void Game::onKeyDown( SDL_KeyboardEvent event )
{
	switch(event.keysym.sym)
	{
		case SDLK_ESCAPE: must_exit = true; break; //ESC key, kill the app
		case SDLK_F1: Shader::ReloadAll(); break; 
	}
}

void Game::onKeyUp(SDL_KeyboardEvent event)
{
	if (event.keysym.sym == SDL_SCANCODE_W) //left mouse unpressed
	{
		
	}
}

void Game::onGamepadButtonDown(SDL_JoyButtonEvent event)
{

}

void Game::onGamepadButtonUp(SDL_JoyButtonEvent event)
{

}

void Game::onMouseButtonDown( SDL_MouseButtonEvent event )
{
	if (event.button == SDL_BUTTON_MIDDLE) //middle mouse
	{
		mouse_locked = !mouse_locked;
		SDL_ShowCursor(!mouse_locked);
	}
}

void Game::onMouseButtonUp(SDL_MouseButtonEvent event)
{
}

void Game::onMouseWheel(SDL_MouseWheelEvent event)
{
	mouse_speed *= event.y > 0 ? 1.1 : 0.9;
}

void Game::onResize(int width, int height)
{
    std::cout << "window resized: " << width << "," << height << std::endl;
	glViewport( 0,0, width, height );
	camera->aspect =  width / (float)height;
	window_width = width;
	window_height = height;
}

