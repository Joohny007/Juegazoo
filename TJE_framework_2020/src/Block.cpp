#include "Block.h"

Block::Block(int id, Vector3 pos)
{
	//this->model.setIdentity();
	this->id = id;
	this->model.setTranslation(pos.x, pos.y, pos.z);

}

void Block::inicialize()
{
	//enable shader and pass uniforms
	this->mesh = Mesh::Get("data/bloquePrueba4.obj");
	this->texture = Texture::Get("data/initialShadingGroup_Base_Color3.png");
	// example of shader loading using the shaders manager
	this->shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
}

void Block::render()
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