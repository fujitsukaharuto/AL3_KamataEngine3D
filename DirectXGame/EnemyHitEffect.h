#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"

class EnemyHitEffect {
public:
	EnemyHitEffect() = default;
	~EnemyHitEffect() = default;

	void Initialize();

	void UpdateWorldTransform();

	void Draw(const ViewProjection& viewProjection);

	void SetPosition(const Vector3& translation);

	void SetParent(const WorldTransform& worldTrans);

	bool GetIsDeath() const ;

private:

	std::unique_ptr<Model> model_ = nullptr;
	WorldTransform worldTransform_;
	float timer_ = 10.0f;
	bool isDeath_ = false;

};