#include "BaseCharacter.h"

BaseCharacter::BaseCharacter() {}

BaseCharacter::~BaseCharacter() {}

void BaseCharacter::Initialize(const std::vector<Model*>& models)
{

	models_ = models;
	worldTransform_.Initialize();

}

void BaseCharacter::Update()
{

	worldTransform_.UpdateMatrix();

}

void BaseCharacter::Draw(const ViewProjection& viewProjection)
{

	for (Model* model : models_) {
		model->Draw(worldTransform_, viewProjection);
	}

}
