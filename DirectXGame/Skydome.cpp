#include "Skydome.h"

Skydome::Skydome() {}

Skydome::~Skydome() {}

void Skydome::Initialize(Model* model) {

	worldTransform_.Initialize();
	model_ = model;
}

void Skydome::Update() {}

void Skydome::Draw(ViewProjection& viewProjection)
{

	model_->Draw(worldTransform_, viewProjection);

}