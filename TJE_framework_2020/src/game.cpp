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

#include <cmath>

//some globals
Mesh* mesh = NULL;
Mesh* mesh2 = NULL;
Mesh* mesh3 = NULL;
Mesh* grass = NULL;
Texture* texture = NULL;
Texture* textureMesh = NULL;
Shader* shader = NULL;
Animation* anim = NULL;
float angle = 0;
float mouse_speed = 10.0f;
FBO* fbo = NULL;

bool free_camera = true;
World world(2);
Player& player = world.player;
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

	//load one texture without using the Texture Manager (Texture::Get would use the manager)
	//texture = new Texture();
 	//texture->load("data/WesternPack/PolygonWestern_Texture_01_A.png");

	// example of loading Mesh from Mesh Manager

	//texture = Texture::Get("data/texture.tga");
	//mesh = Mesh::Get("data/GiantGeneralPack/Animals_T/penguin_20.obj");
	//mesh2 = Mesh::Get("data/bloquePrueba4.obj");

	world.inicializePenguins();
	world.inicializeBlocks();
	world.inicializePlayer();

	//grass = Mesh::Get("data/GiantGeneralPack/Grass_T/grass-long_orange_8.obj");
	textureMesh = Texture::Get("data/initialShadingGroup_Base_Color3.png"); //JOAN CALLATE LA BOCA

	// example of shader loading using the shaders manager
	shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");

	//hide the cursor
	SDL_ShowCursor(!mouse_locked); //hide or show the mouse
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

	player.model.translate(player.pos.x, player.pos.y, player.pos.z);
	player.model.rotate(player.yaw * DEG2RAD, Vector3(0.0f, 1.0f, 0.0f));

	if (!free_camera) {
		Vector3 eye = player.model * Vector3(0.0f, 6.0f, 3.5f);
		Vector3 center = player.model * Vector3(0.0f, 0.0f, -10.0f);
		Vector3 up = Vector3(0.0f, 1.0f, 0.0f);
		camera->lookAt(eye, center, up);
	}

	//set flags
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
   
	//create model matrix for cube

	if (!shader) return;
	
	//enable shader
	shader->enable();

	//upload uniforms
	shader->setUniform("u_color", Vector4(1,1,1,1));
	shader->setUniform("u_viewprojection", camera->viewprojection_matrix );
	shader->setUniform("u_texture", textureMesh, 0);
	shader->setUniform("u_model", Matrix44());
	shader->setUniform("u_time", time);

	//do the draw call
	//mesh->render( GL_TRIANGLES );
	//mesh2->render(GL_TRIANGLES);
	//player.render();
	world.renderPenguins();
	world.renderBlocks();
	world.renderPlayer();

	//disable shader
	shader->disable();

	//Draw the floor grid
	drawGrid();

	//render the FPS, Draw Calls, etc
	drawText(2, 2, getGPUStats(), Vector3(1, 1, 1), 2);

	//swap between front buffer and back buffer
	SDL_GL_SwapWindow(this->window);
}

void Game::update(double seconds_elapsed)
{
	float speed = seconds_elapsed * mouse_speed; //the speed is defined by the seconds_elapsed so it goes constant

	//example
	angle += (float)seconds_elapsed * 10.0f;

	//mouse input to rotate the cam
	if ((Input::mouse_state & SDL_BUTTON_LEFT) || mouse_locked ) //is left button pressed?
	{
		camera->rotate(Input::mouse_delta.x * 0.005f, Vector3(0.0f,-1.0f,0.0f));
		camera->rotate(Input::mouse_delta.y * 0.005f, camera->getLocalVector( Vector3(-1.0f,0.0f,0.0f)));
	}

	if (free_camera) {
		//async input to move the camera around
		if(Input::isKeyPressed(SDL_SCANCODE_LSHIFT) ) speed *= 10; //move faster with left shift
		if (Input::isKeyPressed(SDL_SCANCODE_W) || Input::isKeyPressed(SDL_SCANCODE_UP)) camera->move(Vector3(0.0f, 0.0f, 1.0f) * speed);
		if (Input::isKeyPressed(SDL_SCANCODE_S) || Input::isKeyPressed(SDL_SCANCODE_DOWN)) camera->move(Vector3(0.0f, 0.0f,-1.0f) * speed);
		if (Input::isKeyPressed(SDL_SCANCODE_A) || Input::isKeyPressed(SDL_SCANCODE_LEFT)) camera->move(Vector3(1.0f, 0.0f, 0.0f) * speed);
		if (Input::isKeyPressed(SDL_SCANCODE_D) || Input::isKeyPressed(SDL_SCANCODE_RIGHT)) camera->move(Vector3(-1.0f,0.0f, 0.0f) * speed);
	}
	else {
		float speed = player.speed * elapsed_time;
		float rot_speed = player.rot_speed * elapsed_time;
		float character_radius = 0.5;

		player.model.setRotation(player.yaw * DEG2RAD, Vector3(0, 1, 0));

		Vector3 playerFront = player.model.rotateVector(Vector3(0.0f, 0.0f, -1.0f));
		Vector3 playerRight = player.model.rotateVector(Vector3(1.0f, 0.0f, 0.0f));

		Vector3 playerSpeed;
		if (Input::isKeyPressed(SDL_SCANCODE_LSHIFT)) speed *= 10; //move faster with left shift
		if (Input::isKeyPressed(SDL_SCANCODE_W)) playerSpeed = playerSpeed + (playerFront * speed);
		if (Input::isKeyPressed(SDL_SCANCODE_S)) playerSpeed = playerSpeed + (playerFront * -speed);
		if (Input::isKeyPressed(SDL_SCANCODE_Q)) playerSpeed = playerSpeed + (playerRight * -speed);
		if (Input::isKeyPressed(SDL_SCANCODE_E)) playerSpeed = playerSpeed + (playerRight * speed);

		if (Input::isKeyPressed(SDL_SCANCODE_D)) player.yaw += rot_speed;
		if (Input::isKeyPressed(SDL_SCANCODE_A)) player.yaw -= rot_speed;

		Vector3 targetPos = player.pos + playerSpeed;
		player.pos = targetPos;
		if (world.isPlayeronaBlock(player.pos) == true) {
			player.pos.y = 1;
		}
		else { player.pos.y = 0;}
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

