#include "Enemy.h"
#include <cassert>

#include "MathCal.h"

Enemy::Enemy() {}

Enemy::~Enemy()
{

	for (EnemyBullet* bullet : bullets_) {
		delete bullet;
	}

}

void Enemy::Initialize(Model* model, uint32_t textureHandle)
{
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;
	worldTransform_.Initialize();
	worldTransform_.translation_ = {3.0f, 3.0f, 40.0f};
	
}

void (Enemy::*Enemy::pPhaseTable[])() = {
    &Enemy::ApproachMove,
	&Enemy::LeaveMove
};

void Enemy::Update()
{

	(this->*pPhaseTable[static_cast<size_t>(phase_)])();

	//switch (phase_) {
	//case Enemy::Phase::Approach:
	//default:
	//	ApproachMove();
	//	break;
	//case Enemy::Phase::Leave:
	//	LeaveMove();
	//	break;
	//}

	worldTransform_.UpdateMatrix();

	Fire();
	for (EnemyBullet* bullet : bullets_) {
		bullet->Update();
	}

}

void Enemy::Draw(ViewProjection& viewProjection)
{

	model_->Draw(worldTransform_, viewProjection, textureHandle_);
	for (EnemyBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}

}

void Enemy::ApproachMove()
{

	Vector3 move = {0, 0, 0};
	const float kApproachSpeedZ = 0.2f;
	const Vector3 kApproachSpeed{0.0f, 0.0f, -kApproachSpeedZ};
	move += kApproachSpeed;
	/*if (worldTransform_.translation_.z<0.0f) {
		phase_ = Phase::Leave;
	}*/

	worldTransform_.translation_ += move;

}

void Enemy::LeaveMove()
{

	Vector3 move = {0, 0, 0};
	const float kSpeed = 0.1f;
	const Vector3 kLeaveSpeed{-kSpeed, kSpeed, 0.0f};
	move += kLeaveSpeed;
	worldTransform_.translation_ += move;

}

void Enemy::Fire()
{
	if (--fireTimer_<=0) {
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, -kBulletSpeed);
		velocity = TransformNormal(velocity, worldTransform_.matWorld_);

		EnemyBullet* newBullet = new EnemyBullet();
		newBullet->Initialize(model_, worldTransform_.translation_, velocity);
		bullets_.push_back(newBullet);
		fireTimer_ = kFireTime;
	}

}
