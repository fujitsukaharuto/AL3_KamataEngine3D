#include "EnemyAttackZone.h"
#include <cassert>

EnemyAttackZone::EnemyAttackZone() {



}

EnemyAttackZone::~EnemyAttackZone() {



}

void EnemyAttackZone::Initialize(Model* model, const Vector3& position) {

	assert(model);
	model_ = model;

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;

	worldTransform_.rotation_.x = -1.5708f;
	worldTransform_.scale_ = {1.5f, 1.5f, 1.5f};

	worldTransform_.UpdateMatrix();

	Collider::Initialize();
	Collider::SetTypeID(static_cast<uint32_t>(CollisionTypeIdDef::kEnemy));
	Collider::SetRadius(1.2f);

}

void EnemyAttackZone::Update() {

	deathTimer_--;
	if (deathTimer_ <= 0) {
		isDisappear_ = true;
	}

}

void EnemyAttackZone::Draw(const ViewProjection& viewProjection) {

	model_->Draw(worldTransform_, viewProjection);

}

Vector3 EnemyAttackZone::GetCenterPosition() const { return worldTransform_.translation_; }

void EnemyAttackZone::OnCollision(Collider* other) {

	uint32_t typeID = other->GetTypeID();

	if (typeID == static_cast<uint32_t>(CollisionTypeIdDef::kPlayer) || typeID == static_cast<uint32_t>(CollisionTypeIdDef::kSnowBall)) {
		isDisappear_ = true;
	}

}
