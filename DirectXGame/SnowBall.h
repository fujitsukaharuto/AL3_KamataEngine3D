#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "Collider.h"
#include "CollisionTypeIdDef.h"

class LittleEnemy;

class SnowBall: public Collider {
public:
	SnowBall();
	~SnowBall();

	void Initialize(Model* model, const Vector3& position);

	void Update();

	void Draw(const ViewProjection& viewProjection);

	bool IsDead() const { return isDead_; }

	bool IsRemove() const { return isRemove_; }

	Vector3 GetCenterPosition() const override;

	float GetRadius();

	Vector3 GetTrans();

	Matrix4x4 GetWorldMat();

	void OnCollision([[maybe_unused]] Collider* other) override;

	void SetTransform(const Vector3& trans);

	void SetVelocity(const Vector3& velo);

	void SetRemove(bool isRe);

	void SetRotate(const Vector3& rotate);

	void SetSizeRadius(const float rad);

	void SetLittleModel(std::vector<Model*> model);

private:

	Model* model_ = nullptr;
	WorldTransform worldTransform_;

	Vector3 velocity_;
	int32_t deathTimer_ = 120;

	float sizeRadius_ = 0.5f;
	float addRadius_ = 0.5f;

	bool isRemove_ = false;
	bool isDead_ = false;

	std::vector<Model*> littleModel_;
	uint32_t littleEnemyCount_ = 0;
	std::list<LittleEnemy*> littleEnemys_;
};
