#include "LittleEnemy.h"
#include "MathCal.h"
#include "SnowBall.h"
#include "CollisionTypeIdDef.h"

LittleEnemy::LittleEnemy() {}

LittleEnemy::~LittleEnemy() {}

void LittleEnemy::Initialize(const std::vector<Model*>& models) {

	BaseCharacter::Initialize(models);
	worldTransformBody_.Initialize();

	worldTransformBody_.parent_ = &worldTransform_;


	worldTransform_.translation_ = {0.0f, 0.0f, 0.0f};
	worldTransformBody_.translation_ = {0.0f, 0.5f, 0.0f};
	worldTransform_.scale_ = {0.7f, 0.7f, 0.7f};

	worldTransform_.UpdateMatrix();
	worldTransformBody_.UpdateMatrix();

	Collider::SetTypeID(static_cast<uint32_t>(CollisionTypeIdDef::kLittleEnemy));
	Collider::SetRadius(0.5f);

}

void LittleEnemy::Update() {
	if (!isGetCaughtUp_) {
		Move();
	}

	BaseCharacter::Update();
	worldTransformBody_.UpdateMatrix();
}

void LittleEnemy::Draw(const ViewProjection& viewProjection) {
	models_[0]->Draw(worldTransformBody_, viewProjection);
}

void LittleEnemy::Move() {

	const float kSpeed = 0.2f;

	targetPosition_.y = 0.0f;
	Vector3 toTarget = targetPosition_ - GetCenterPosition();
	Vector3 norToTarget = toTarget.Normalize();
	Vector3 norVelocity = norToTarget * kSpeed;

	Vector3 velocity;
	velocity = Sleap(norVelocity, norToTarget, 0.025f) * 1.0f;

	worldTransform_.translation_ += velocity;
	worldTransform_.rotation_.y = std::atan2(velocity.x, velocity.z);

}

void LittleEnemy::OnCollision(Collider* other) {
	uint32_t typeID = other->GetTypeID();

	if (typeID == static_cast<uint32_t>(CollisionTypeIdDef::kSnowBall)) {
		isDead_ = true;
	}
}

Vector3 LittleEnemy::GetCenterPosition() const {
	const Vector3 offset = {0.0f, 0.0f, 0.0f};
	Vector3 worldPos = Transform(offset, worldTransform_.matWorld_);
	return worldPos;
}

Vector3 LittleEnemy::GetOrigineScale() { return {0.5f, 0.5f, 0.5f}; }

void LittleEnemy::Setparent(const WorldTransform& pare) { worldTransform_.parent_ = &pare; }

void LittleEnemy::SetRotate(const Vector3& rotate) { worldTransform_.rotation_ = rotate; }

void LittleEnemy::SetSclae(const Vector3& scale) { worldTransform_.scale_ = scale; }

void LittleEnemy::SetIsGetCaught(const bool is) { isGetCaughtUp_ = is; }

void LittleEnemy::SetPosition(const Vector3& pos) { worldTransform_.translation_ = pos; }

void LittleEnemy::SetBodyPosition(const Vector3& pos) { worldTransformBody_.translation_ = pos; }

void LittleEnemy::SetTargetPosision(const Vector3& target) { targetPosition_ = target; }
