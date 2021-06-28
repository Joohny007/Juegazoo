
#pragma once
#include "mesh.h"
#include "texture.h"
#include "Entity.h"
#include "camera.h"
#include "shader.h"
#include "Player.h"
#include "Block.h"
#include "Penguin.h"
#include "animation.h"
#include "game.h"
#include "extra/bass.h"


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
	std::vector<Player> players;
	Player player;
	float game_time;
	int pingu_counter;
	int max_penguins;
	int max_blocks;
	Mesh* sky_mesh;
	Texture* sky_tex;
	Matrix44 sky_model;
	Shader* sky_shader;
	Mesh* sea_mesh;
	Texture* sea_tex;
	Matrix44 sea_model;
	Shader* sea_shader;

	World(int x);
	void inicializePenguins();
	void renderPenguins(bool renderBoundings, Camera* camera);
	void inicializeBlocks();
	void renderBlocks(bool renderBoundings, Camera* camera);
	void inicializePlayers();
	void renderPlayer1(Camera* camera);
	void renderPlayer2(Camera* camera);
	void inicializeSky();
	void renderSky(Camera* camera);
	void inicializeSea();
	void renderSea(Camera* camera);
	float isPlayeronaBlock(Vector3 playerpos);
	void BlockVibration( float et);
	void penguinCollision(Player& player, Vector3& targetPos, float elapsed_time, float time_float);
	float easeOutQuint(float x);
	void kickAnimation1(Camera* camera, Shader* skinning, Shader* shader, Texture* textureMesh, Animation* anim, float time_float);
	void kickAnimation2(Camera* player2Cam, Shader* skinning, Shader* shader, Texture* textureMesh, Animation* anim, float time_float);
	void moving1(Camera* camera, Shader* skinning, Shader* shader, Texture* textureMesh, Animation* anim, float time_float);
	void moving2(Camera* player2Cam, Shader* skinning, Shader* shader, Texture* textureMesh, Animation* anim, float time_float);
	void stunPlayer1(Vector3& targetPos, float time);
	void stunPlayer2(Vector3& targetPos, float time);
	void renderGUI(Camera* cam);
	void playSound(const char* sound);
};