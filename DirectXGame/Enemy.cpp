#include "Enemy.h"
#include <cassert>

Enemy::Enemy() {}

Enemy::~Enemy() {}

void Enemy::Initialize(Model* model, uint32_t textureHandle)
{
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;
	worldTransform_.Initialize();
	worldTransform_.translation_ = {0.0f, 3.0f, 20.0f};

}

void Enemy::Update()
{

	switch (phase_) {
	case Enemy::Phase::Approach:
	default:
		ApproachMove();
		break;
	case Enemy::Phase::Leave:
		LeaveMove();
		break;
	}

}

void Enemy::Draw(ViewProjection& viewProjection)
{

	model_->Draw(worldTransform_, viewProjection, textureHandle_);

}

void Enemy::ApproachMove()
{

	Vector3 move = {0, 0, 0};
	const float kApproachSpeedZ = 0.2f;
	const Vector3 kApproachSpeed{0.0f, 0.0f, -kApproachSpeedZ};
	move += kApproachSpeed;
	if (worldTransform_.translation_.z<0.0f) {
		phase_ = Phase::Leave;
	}

	worldTransform_.translation_ += move;
	worldTransform_.UpdateMatrix();
}

void Enemy::LeaveMove()
{

	Vector3 move = {0, 0, 0};
	const float kSpeed = 0.1f;
	const Vector3 kLeaveSpeed{-kSpeed, kSpeed, 0.0f};
	move += kLeaveSpeed;
	worldTransform_.translation_ += move;
	worldTransform_.UpdateMatrix();
}
