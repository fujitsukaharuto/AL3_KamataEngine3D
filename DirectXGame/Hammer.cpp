#include "Hammer.h"
#include "MathCal.h"
#include "Enemy.h"
#include "CollisionTypeIdDef.h"

Hammer::~Hammer() {

	for (EnemyHitEffect* effect : enemyEffects_) {
		delete effect;
	}

}

void Hammer::Initialize(Model* model) {

	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.translation_ = {0.0f, 0.9f, -0.3f};
	worldTransform_.UpdateMatrix();
	Collider::Initialize();
	Collider::SetTypeID(static_cast<uint32_t>(CollisionTypeIdDef::kPlayerWeapon));

}

void Hammer::UpdateWorldTransform() {

	enemyEffects_.remove_if([](EnemyHitEffect* effect) {
		if (effect->GetIsDeath()) {
			delete effect;
			return true;
		}
		return false;
	});

	worldTransform_.UpdateMatrix();

	for (EnemyHitEffect* effect : enemyEffects_) {
		effect->UpdateWorldTransform();
	}

}

void Hammer::Draw(const ViewProjection& viewProjection) {

	model_->Draw(worldTransform_, viewProjection);
	/*enemyEffects_.reverse();*/
	for (EnemyHitEffect* effect : enemyEffects_) {
		effect->Draw(viewProjection);
	}
	/*enemyEffects_.reverse();*/
}

void Hammer::OnCollision([[maybe_unused]] Collider* other) {

	uint32_t typeID = other->GetTypeID();
	// 衝突相手が敵なら
	if (typeID == static_cast<uint32_t>(CollisionTypeIdDef::kEnemy)) {
		Enemy* enemy = static_cast<Enemy*>(other);
		uint32_t serialNumber = enemy->GetSerialNumber();

		// 接触履歴があれば何もしない
		if (collisionRecord_.CheckHistory(serialNumber)) {
			return;
		}
		collisionRecord_.AddRecord(serialNumber);

		// 敵の位置にエフェクト
		EnemyHitEffect* effect = new EnemyHitEffect();
		effect->Initialize();
		effect->SetParent(enemy->GetWorldTransform());
		effect->UpdateWorldTransform();
		enemyEffects_.push_back(effect);
	}

}

Vector3 Hammer::GetCenterPosition() const {

	const Vector3 offset = {0.0f, 4.5f, 0.0f};

	Vector3 worldPos = Transform(offset, worldTransform_.matWorld_);

	return worldPos;
}

void Hammer::SetParent(const WorldTransform& worldTrans) {

	worldTransform_.parent_ = &worldTrans;

}

const Vector3& Hammer::GetRotation() const { return worldTransform_.rotation_; }

void Hammer::SetRotaion(const Vector3& rotation) { worldTransform_.rotation_ = rotation; }
