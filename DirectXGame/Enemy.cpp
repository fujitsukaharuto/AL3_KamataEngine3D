#include "Enemy.h"
#include <cassert>

#include "MathCal.h"
#include "Player.h"

Enemy::Enemy() {}

Enemy::~Enemy()
{

	for (TimedCall* timedcal : timedCalls_) {
		delete timedcal;
	}

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
	worldTransform_.UpdateMatrix();

	/*InitApproach();*/

}

void (Enemy::*Enemy::pPhaseTable[])() = {
    &Enemy::ApproachMove,
	&Enemy::LeaveMove
};

void Enemy::Update()
{
	timedCalls_.remove_if([](TimedCall* timedcall) {
		if (timedcall->IsFinished()) {
			delete timedcall;
			return true;
		}
		return false;
	});

	bullets_.remove_if([](EnemyBullet* enemyBullet) {
		if (enemyBullet->IsDead()) {
			delete enemyBullet;
			return true;
		}
		return false;
	});


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

	//if (--fireTimer_ <= 0) {
	//	Fire();
	//	fireTimer_ = kFireTime;
	//}
	
	for (TimedCall* timedcal : timedCalls_) {
		timedcal->Update();
	}

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
	//if (worldTransform_.translation_.z<0.0f) {
	//	phase_ = Phase::Leave;
	//	timedCalls_.clear();
	//}

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

	const float kBulletSpeed = 1.0f;

	Vector3 playerPos = player_->GetworldPosition();
	Vector3 enemyPos = GetWorldPosition();
	Vector3 bulletDire = playerPos - enemyPos;
	Vector3 velocity = bulletDire.Normalize();
	velocity = velocity * kBulletSpeed;

	//Vector3 velocity(0, 0, -kBulletSpeed);
	//velocity = TransformNormal(velocity, worldTransform_.matWorld_);

	EnemyBullet* newBullet = new EnemyBullet();
	newBullet->Initialize(model_, worldTransform_.translation_, velocity);
	bullets_.push_back(newBullet);

}

void Enemy::FireRoop()
{

	Fire();
	timedCalls_.push_back(new TimedCall(std::bind(&Enemy::FireRoop, this), 60));

}

void Enemy::InitApproach() 
{

	fireTimer_ = 10;
	FireRoop();

}

Vector3 Enemy::GetWorldPosition()
{
	Vector3 worldPos;
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}
