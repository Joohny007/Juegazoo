#include "Block.h"

Block::Block(int id, Vector3 pos)
{
	//this->model.setIdentity();
	this->id = id;
	this->model.setTranslation(pos.x, pos.y, pos.z);
	this->dir = DOWN;
	this->vel = 5.0f;
	this->fallen = false;

}

void Block::inicialize()
{
	//enable shader and pass uniforms
	if (this->fallen == false) {
		this->mesh = Mesh::Get("data/bloquePrueba5.obj");
		this->texture = Texture::Get("data/initialShadingGroup_Base_Color3.png");
		// example of shader loading using the shaders manager
		this->shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
	}
}

void Block::inicialize32()
{
	if (this->fallen == false) {
		this->mesh = Mesh::Get("data/bloquePrueba5.obj");
		this->texture = Texture::Get("data/asul.png");
		// example of shader loading using the shaders manager
		this->shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
	}
}

void Block::inicialize23()
{
	if (this->fallen == false) {
		this->mesh = Mesh::Get("data/bloquePrueba5.obj");
		this->texture = Texture::Get("data/rojo.png");
		// example of shader loading using the shaders manager
		this->shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
	}
}

void Block::render(Camera* camera)
{
	//get the last camera that was activated
	if (this->fallen == false) {
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
}
void Block::vibrate(float et) {
	Vector3 pos = this->model.getTranslation();
	if ((this->vel >= 10) && (pos.y >= 1)) {
		this->vel = 2;
	}
	else if (this->vel == 2) {
		pos.y -= (this->vel) * et;
	}
	else {
		if (pos.y >= 1) { this->dir = DOWN; this->vel += 0.2; }
		else if (pos.y <= -2) { this->dir = UP; }
		if (this->dir == DOWN) {
			pos.y -= (this->vel) * et;
		}
		else if (this->dir == UP) {
			pos.y += (this->vel) * et;
		}
	}
	this->model.setTranslation(pos.x, pos.y, pos.z);
}