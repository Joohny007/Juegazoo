#include "Player.h"

Player::Player()
{
	this->speed = 10.0f;
	this->rot_speed = 50.0f;
	this->yaw = 0;
	this->pos = model.getTranslation();
	this->mesh = NULL;
	this->shader = NULL;
	this->texture = NULL;
	this->dir = FORWARD;
	this->kick = false;
	this->moving = false;
	this->stunned = false;
	this->stun_counter = 0.0f;
	this->kick_cooldown = 3.5f;
	this->score = 0;
}
Player::Player(int id)
{
	//this->model.setIdentity();
	this->id = id;
	this->speed = 10.0f;
	this->rot_speed = 50.0f;
	this->yaw = 0;
	this->pos = model.getTranslation();
	this->mesh = NULL;
	this->shader = NULL;
	this->texture = NULL;
	this->dir = FORWARD;
	this->kick = false;
	this->moving = false;
	this->stunned = false;
	this->stun_counter = 0.0f;
	this->kick_cooldown = 3.5f;
	this->score = 0;
}

void Player::setPos(Vector3 pos)
{
	this->model.translate(pos.x, pos.y, pos.z);
	this->pos = pos;
}

void Player::inicialize()
{
	//enable shader and pass uniforms
	this->mesh = Mesh::Get("data/GiantGeneralPack/People_T/woman_viking.mesh");
	this->texture = Texture::Get("data/GiantGeneralPack/color-atlas-new.png"); 
	// example of shader loading using the shaders manager
	this->shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
}

void Player::inicialize2()
{
	//enable shader and pass uniforms
	this->mesh = Mesh::Get("data/GiantGeneralPack/People_T/Viking_nice.mesh");
	this->texture = Texture::Get("data/GiantGeneralPack/color-atlas-new.png"); 
	// example of shader loading using the shaders manager
	this->shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
}

void Player::render(Camera* camera)
{
	//get the last camera that was activated
	//Camera* camera = Camera::current;
	Matrix44& model = this->model;

	//enable shader and pass uniforms
	shader->enable();
	shader->setUniform("u_model", model);
	shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
	shader->setTexture("u_texture", texture->texture_id);

	//render the mesh using the shader
	mesh->render(GL_TRIANGLES);

	//disable the shader after finishing rendering
	shader->disable();
}


std::string Player::scoreToString()
{
	return std::to_string(score);
}