#include "EnemyBullet.h"
#include "TextureManager.h"
#include "MathCal.h"
#include "Player.h"
#include <cassert>

EnemyBullet::EnemyBullet() {}

EnemyBullet::~EnemyBullet() {}

void EnemyBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity)
{

	assert(model);
	model_ = model;
	textureHandle_ = TextureManager::Load("red1x1.png");
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.scale_.x = 0.5f;
	worldTransform_.scale_.y = 0.5f;
	worldTransform_.scale_.z = 3.0f;

	velocity_ = velocity;

	SetAttribute(kCollisionAttributeEnemy);
	SetMask((0xffffffff ^ (kCollisionAttributeEnemy)));

	worldTransform_.rotation_.y = std::atan2(velocity_.x, velocity_.z);
	Matrix4x4 yrota = MakeRotateYMatrix(-worldTransform_.rotation_.y);
	Vector3 velocityZ = TransformNormal(velocity_, yrota);
	worldTransform_.rotation_.x = std::atan2(-velocityZ.y, velocityZ.z);
	worldTransform_.UpdateMatrix();
}

void EnemyBullet::Update()
{
	Vector3 toPlayer = player_->GetWorldPosition() - GetWorldPosition();
	Vector3 norToPlayer = toPlayer.Normalize();
	Vector3 norVelocity = velocity_.Normalize();

	velocity_ = Sleap(norVelocity, norToPlayer, 0.025f) * 1.0f;

	worldTransform_.rotation_.y = std::atan2(velocity_.x, velocity_.z);
	Matrix4x4 yrota = MakeRotateYMatrix(-worldTransform_.rotation_.y);
	Vector3 velocityZ = TransformNormal(velocity_, yrota);
	worldTransform_.rotation_.x = std::atan2(-velocityZ.y, velocityZ.z);
	worldTransform_.UpdateMatrix();

	worldTransform_.translation_ += velocity_;
	worldTransform_.UpdateMatrix();
	if (--deathTimer_<=0) {
	
		isDead_ = true;
	
	}

}

void EnemyBullet::Draw(const ViewProjection& viewProjection)
{

	model_->Draw(worldTransform_, viewProjection, textureHandle_);

}

Vector3 EnemyBullet::GetWorldPosition() {
	Vector3 worldPos;
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

void EnemyBullet::OnCollision() { isDead_ = true; }
