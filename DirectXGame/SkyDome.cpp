#include "SkyDome.h"

SkyDome::SkyDome() {}

SkyDome::~SkyDome() {}

void SkyDome::Initialize(Model* model) {
	
	worldTransform_.Initialize();
	model_ = model;
}

void SkyDome::Update() {}

void SkyDome::Draw(ViewProjection& viewProjection) {

	model_->Draw(worldTransform_, viewProjection);

}
