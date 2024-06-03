#include "Ground.h"

Ground::Ground() {}

Ground::~Ground() {}

void Ground::Initialize(Model* model)
{

	worldTransform_.Initialize();
	model_ = model;

}

void Ground::Update() {}

void Ground::Draw(ViewProjection& viewProjection)
{

	model_->Draw(worldTransform_, viewProjection);

}
