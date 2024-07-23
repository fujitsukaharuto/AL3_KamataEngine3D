#pragma once
#include "Collider.h"
#include "EnemyHitEffect.h"
#include "CollisionRecord.h"

#include <list>

class Hammer final : public Collider {
public:
	Hammer() = default;
	~Hammer();

	void Initialize(Model* model);

	void UpdateWorldTransform();

	void Draw(const ViewProjection& viewProjection);

	void OnCollision([[maybe_unused]] Collider* other) override;

	Vector3 GetCenterPosition() const override;

	void SetParent(const WorldTransform& worldTransform);

	const Vector3& GetRotation() const;

	void SetRotaion(const Vector3& rotation);

private:

	Model* model_ = nullptr;
	WorldTransform worldTransform_;
	std::list<EnemyHitEffect*> enemyEffects_;
	CollisionRecord collisionRecord_;

};