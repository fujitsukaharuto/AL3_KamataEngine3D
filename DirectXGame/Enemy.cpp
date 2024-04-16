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
	worldTransform_.translation_ = {0.0f, 3.0f, 10.0f};

}

void Enemy::Update()
{

	Vector3 move = {0, 0, 0};
	const float kEnemySpeed = 0.2f;
	move.z -= kEnemySpeed;

	worldTransform_.translation_ += move;

	worldTransform_.UpdateMatrix();

}

void Enemy::Draw(ViewProjection& viewProjection)
{

	model_->Draw(worldTransform_, viewProjection, textureHandle_);

}
