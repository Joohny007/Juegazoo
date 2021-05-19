#include "Player.h"

Player::Player()
{
	this->model.setIdentity();
}

void Player::render()
{
	//get the last camera that was activated
	Camera* camera = Camera::current;
	Matrix44 model = this->model;
	Mesh* mesh = NULL;

	//enable shader and pass uniforms
	shader->enable();
	shader->setUniform("u_model", model);
	shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
	shader->setTexture("u_texture", texture->texture_id);

	//render the mesh using the shader
	mesh = Mesh::Get("data/GiantGeneralPack/Animals_T/penguin_20.obj");
	mesh->render(GL_TRIANGLES);

	//disable the shader after finishing rendering
	shader->disable();
}