#include "PlayerBullet.h"
#include "TextureManager.h"
#include<cassert>

PlayerBullet::PlayerBullet() {}

PlayerBullet::~PlayerBullet() {}

void PlayerBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity) {
	assert(model);
	model_ = model;
	textureHandle_ = TextureManager::Load("white1x1.png");

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;

	velocity_ = velocity;

}

void PlayerBullet::Update()
{

	worldTransform_.translation_ += velocity_;
	worldTransform_.UpdateMatrix();

	if (--deathTimer_<=0) {
		isDead_ = true;
	}

}

void PlayerBullet::Draw(const ViewProjection& viewProjection)
{

	model_->Draw(worldTransform_, viewProjection, textureHandle_);

}

Vector3 PlayerBullet::GetWorldPos()
{
	Vector3 worldPos;
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

void PlayerBullet::OnCollision() { isDead_ = true; }
