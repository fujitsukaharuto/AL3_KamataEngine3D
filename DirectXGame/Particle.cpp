#include "Particle.h"
#include "MathCal.h"
#include <cassert>


Particle::Particle() {}

Particle::~Particle() {}

void Particle::Initialize(Model* model, Vector3 velocity, Vector3 position) {
	assert(model);
	model_ = model;

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;

	velocity_ = velocity;

	color_.Initialize();
	color_.TransferMatrix();
}

void Particle::Update() {

	if (liveTime_ == 0) {
		isDead_ = true;
	} else {
		liveTime_--;
		if (isGravity_) {
			velocity_.y += -0.025f;
		}
		worldTransform_.translation_ += velocity_;
		worldTransform_.UpdateMatrix();
	}
}

void Particle::Draw(const ViewProjection& viewProjection) {

	model_->Draw(worldTransform_, viewProjection, &color_);

}

void Particle::SetColor(const Vector4& color) {

	color_.SetColor(color);
	color_.TransferMatrix();
}

void Particle::SetSize(const float size) {

	worldTransform_.scale_ = {size, size, size};
	worldTransform_.UpdateMatrix();

}
