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

//some globals JOAN ERES UNA BELLISIMA PERSONA ESPERO QUE TE VAYA TODO BIEN EN LA VIDA PRECIOSO
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

void RenderFirstCam(Camera* camera, Camera* player2Cam, float time_float)
{
	glViewport(0, 0, Game::instance->window_width / 2, Game::instance->window_height);
	
	world.renderSky(camera);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	world.renderSea(camera);
	glDisable(GL_BLEND);

	if (!free_camera) {
		player1.model.setTranslation(player1.pos.x, player1.pos.y, player1.pos.z);
		player2.model.setTranslation(player2.pos.x, player2.pos.y, player2.pos.z);
		Vector3 eye1 = player1.model * Vector3(0.0f, 8.0f, -5.5f);
		Vector3 center1 = player1.model * Vector3(0.0f, 0.0f, 10.0f);
		Vector3 up = Vector3(0.0f, 1.0f, 0.0f);
		camera->lookAt(eye1, center1, up);
	}
	Animation* anim_woman = Animation::Get("data/Animaciones/Skanim/mma_kick_woman.skanim");
	Animation* walk_anim_woman = Animation::Get("data/Animaciones/Skanim/walking_woman.skanim");


	world.moving(player1, camera, player2Cam, skinning, shader, textureMesh, walk_anim_woman, time_float);


	world.kickAnimation(player1, camera, player2Cam, skinning, anim_woman, time_float);

	world.renderBlocks(renderBoundings, camera);
	world.renderPenguins(renderBoundings, camera);

}

void RenderSecondCam(Camera* camera, Camera* player2Cam, float time_float)
{
	glViewport(Game::instance->window_width / 2, 0, Game::instance->window_width / 2, Game::instance->window_height);

	world.renderSky(player2Cam);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	world.renderSea(player2Cam);
	glDisable(GL_BLEND);

	if (!free_camera) {
		player1.model.setTranslation(player1.pos.x, player1.pos.y, player1.pos.z);
		player2.model.setTranslation(player2.pos.x, player2.pos.y, player2.pos.z);
		Vector3 eye2 = player2.model * Vector3(0.0f, 8.0f, -5.5f);
		Vector3 center2 = player2.model * Vector3(0.0f, 0.0f, 10.0f);
		Vector3 up = Vector3(0.0f, 1.0f, 0.0f);
		player2Cam->lookAt(eye2, center2, up);
	}
	Animation* anim = Animation::Get("data/Animaciones/Skanim/mma_kick.skanim");
	Animation* walk_anim = Animation::Get("data/Animaciones/Skanim/walking.skanim");
	walk_anim->duration = 1.03;
	
	world.moving(player2, camera, player2Cam, skinning, shader, textureMesh, walk_anim, time_float);
	world.kickAnimation(player2, camera, player2Cam, skinning, anim, time_float);

	world.renderBlocks(renderBoundings, player2Cam);
	world.renderPenguins(renderBoundings, player2Cam);

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

	//set flags
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
   

	if (!shader) return;

	//do the draw call
	//mesh->render( GL_TRIANGLES );
	//mesh2->render(GL_TRIANGLES);
	//player.render();
	/*world.renderPlayer(camera);
	world.renderBlocks(renderBoundings);
	world.renderPenguins(renderBoundings);*/

	RenderFirstCam(camera, player2Cam, time);
	RenderSecondCam(camera, player2Cam, time);

	//disable shader
	//shader->disable();

	

	//Draw the floor grid
	drawGrid();

	//render the FPS, Draw Calls, etc
	drawText(2, 2, getGPUStats(), Vector3(1, 1, 1), 2);

	//swap between front buffer and back buffer
	SDL_GL_SwapWindow(this->window);
}

void Game::update(double seconds_elapsed)
{
	//example
	angle += (float)seconds_elapsed * 10.0f;

	//mouse input to rotate the cam
	if ((Input::mouse_state & SDL_BUTTON_LEFT) || mouse_locked ) //is left button pressed?
	{
		camera->rotate(Input::mouse_delta.x * 0.005f, Vector3(0.0f,-1.0f,0.0f));
		camera->rotate(Input::mouse_delta.y * 0.005f, camera->getLocalVector( Vector3(-1.0f,0.0f,0.0f)));
	}

	if (free_camera) {
		float speed = seconds_elapsed * mouse_speed; //the speed is defined by the seconds_elapsed so it goes constant
		//async input to move the camera around
		if(Input::isKeyPressed(SDL_SCANCODE_LSHIFT) ) speed *= 10; //move faster with left shift
		if (Input::isKeyPressed(SDL_SCANCODE_W) || Input::isKeyPressed(SDL_SCANCODE_UP)) camera->move(Vector3(0.0f, 0.0f, 1.0f) * speed);
		if (Input::isKeyPressed(SDL_SCANCODE_S) || Input::isKeyPressed(SDL_SCANCODE_DOWN)) camera->move(Vector3(0.0f, 0.0f,-1.0f) * speed);
		if (Input::isKeyPressed(SDL_SCANCODE_A) || Input::isKeyPressed(SDL_SCANCODE_LEFT)) camera->move(Vector3(1.0f, 0.0f, 0.0f) * speed);
		if (Input::isKeyPressed(SDL_SCANCODE_D) || Input::isKeyPressed(SDL_SCANCODE_RIGHT)) camera->move(Vector3(-1.0f,0.0f, 0.0f) * speed);
	}
	else {
		float speed1 = player1.speed * elapsed_time;
		float rot_speed1 = player1.rot_speed * elapsed_time;
		float character_radius1 = 0.5;

		//player.model.rotate(player.yaw * DEG2RAD, Vector3(0, 1, 0));

		Vector3& player1Front = player1.model.rotateVector(Vector3(0.0f, 0.0f, -1.0f));
		Vector3& player1Right = player1.model.rotateVector(Vector3(1.0f, 0.0f, 0.0f));

		//Vector3 playerSpeed;
		player1.playerSpeed = Vector3(0, 0, 0);
		if (Input::isKeyPressed(SDL_SCANCODE_LSHIFT)) speed1 *= 10; //move faster with left shift
		if (Input::isKeyPressed(SDL_SCANCODE_W)) {
			player1.playerSpeed = player1.playerSpeed + (player1Front * -speed1);
			player1.dir = Player::type::FORWARD;
			player1.moving = true;
			time = 0;
			/*world.player.model.rotate(180 * DEG2RAD, Vector3(0, 1, 0));*/
		}
		if (Input::isKeyPressed(SDL_SCANCODE_S)) {
			player1.playerSpeed = player1.playerSpeed + (player1Front * speed1);
			player1.dir = Player::type::BACKWARD;
			player1.moving = true;
			time = 0;
			/*world.player.model.rotate(0 * DEG2RAD, Vector3(0, 1, 0));*/
		}
		if (Input::isKeyPressed(SDL_SCANCODE_A)) {
			player1.playerSpeed = player1.playerSpeed + (player1Right * speed1);
			player1.dir = Player::type::LEFT;
			player1.moving = true;
			time = 0;
			/*world.player.model.rotate(90 * DEG2RAD, Vector3(0, 1, 0));*/
		}
		if (Input::isKeyPressed(SDL_SCANCODE_D)) {
			player1.playerSpeed = player1.playerSpeed + (player1Right * -speed1);
			player1.dir = Player::type::RIGHT;
			player1.moving = true;
			time = 0;
			/*world.player.model.rotate(-90 * DEG2RAD, Vector3(0, 1, 0));*/
		}
		if (Input::wasKeyPressed(SDL_SCANCODE_F)) {
			player1.kick = true;
			time = 0;
		}

		if (Input::isKeyPressed(SDL_SCANCODE_Q)) player1.yaw -= rot_speed1;
		if (Input::isKeyPressed(SDL_SCANCODE_E)) player1.yaw += rot_speed1;


		//---------------------------------------------------------------------------------------------------------
		float speed2 = player2.speed * elapsed_time;
		float rot_speed2 = player2.rot_speed * elapsed_time;
		float character_radius2 = 0.5;

		//player.model.rotate(player.yaw * DEG2RAD, Vector3(0, 1, 0));

		Vector3& player2Front = player2.model.rotateVector(Vector3(0.0f, 0.0f, -1.0f));
		Vector3& player2Right = player2.model.rotateVector(Vector3(1.0f, 0.0f, 0.0f));

		//Vector3 playerSpeed2;
		player2.playerSpeed = Vector3(0, 0, 0);
		if (Input::isKeyPressed(SDL_SCANCODE_LSHIFT)) speed2 *= 10; //move faster with left shift
		if (Input::isKeyPressed(SDL_SCANCODE_UP)) {
			player2.playerSpeed = player2.playerSpeed + (player2Front * -speed2);
			player2.dir = Player::type::FORWARD;
			player2.moving = true;
			/*world.player.model.rotate(180 * DEG2RAD, Vector3(0, 1, 0));*/
		}
		if (Input::isKeyPressed(SDL_SCANCODE_DOWN)) {
			player2.playerSpeed = player2.playerSpeed + (player2Front * speed2);
			player2.dir = Player::type::BACKWARD;
			player2.moving = true;
			/*world.player.model.rotate(0 * DEG2RAD, Vector3(0, 1, 0));*/
		}
		if (Input::isKeyPressed(SDL_SCANCODE_LEFT)) {
			player2.playerSpeed = player2.playerSpeed + (player2Right * speed2);
			player2.dir = Player::type::LEFT;
			player2.moving = true;
			/*world.player.model.rotate(90 * DEG2RAD, Vector3(0, 1, 0));*/
		}
		if (Input::isKeyPressed(SDL_SCANCODE_RIGHT)) {
			player2.playerSpeed = player2.playerSpeed + (player2Right * -speed2);
			player2.dir = Player::type::RIGHT;
			player2.moving = true;
			/*world.player.model.rotate(-90 * DEG2RAD, Vector3(0, 1, 0));*/
		}
		if (Input::wasKeyPressed(SDL_SCANCODE_P)) {
			player2.kick = true;
		}

		if (Input::isKeyPressed(SDL_SCANCODE_I)) player2.yaw -= rot_speed2;
		if (Input::isKeyPressed(SDL_SCANCODE_O)) player2.yaw += rot_speed2;


		for (int i = 0; i < world.players.size(); i++) {
			Player& player = world.players[i];
			Vector3& playerSpeed = world.players[i].playerSpeed;
			Vector3& targetPos = world.players[i].pos + playerSpeed;

			world.penguinCollision(player, targetPos, elapsed_time);
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
			Vector3 trans = penguin.pos;
			float checker = world.isPlayeronaBlock(penguin.pos);
			if (checker == -5) {
				penguin.pos.y -= penguin.speed * elapsed_time;
			}
			else {
				penguin.pos.y = checker + 2;
				
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

