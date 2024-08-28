#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "Collider.h"
#include "CollisionTypeIdDef.h"


class EnemyAttackZone : public Collider{
public:
	EnemyAttackZone();
	~EnemyAttackZone();

	void Initialize(Model* model, const Vector3& position);

	void Update();

	void Draw(const ViewProjection& viewProjection);

	Vector3 GetCenterPosition() const override;

	void OnCollision([[maybe_unused]] Collider* other) override;

	bool IsDisappear() const { return isDisappear_; }

private:

	Model* model_ = nullptr;
	WorldTransform worldTransform_;

	int32_t deathTimer_ = 120;
	bool isDisappear_ = false;

};
