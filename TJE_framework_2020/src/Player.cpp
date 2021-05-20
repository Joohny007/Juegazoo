#include "Player.h"

Player::Player(int x)
{
	//this->model.setIdentity();
	this->mesh = NULL;
	this->shader = NULL;
	this->texture = NULL;
	
}

void Player::inicialize()
{
	//enable shader and pass uniforms
	this->mesh = Mesh::Get("data/GiantGeneralPack/Animals_T/penguin_20.obj");
	this->texture = Texture::Get("data/GiantGeneralPack/color-atlas-new.png"); //JOAN CALLATE LA BOCA
	// example of shader loading using the shaders manager
	this->shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
}

void Player::render()
{
	//get the last camera that was activated
	Camera* camera = Camera::current;
	Matrix44 model = this->model;

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