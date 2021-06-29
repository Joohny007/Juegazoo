#include "Penguin.h"

Penguin::Penguin(int id, Vector3 pos)
{
	this->id = id;
	this->speed = 10.0f;
	this->model.translate(pos.x, pos.y, pos.z);
	this->dir = BACKWARD;
	this->pos = pos;
	this->fallen = false;
	this->isMoving = false;
	this->acumulative = 0.0f;
}

Penguin::Penguin()
{
	this->speed = 10.0f;
	this->dir = BACKWARD;
	this->fallen = false;
	this->isMoving = false;
	this->acumulative = 0.0f;
}

void Penguin::inicialize()
{
	//enable shader and pass uniforms
	this->mesh = Mesh::Get("data/GiantGeneralPack/Animals_T/penguin.mesh");
	this->texture = Texture::Get("data/GiantGeneralPack/color-atlas-new.png"); //JOAN CALLATE LA BOCA
	// example of shader loading using the shaders manager
	this->shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
}

void Penguin::render(Camera* camera)
{
	//get the last camera that was activated
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
