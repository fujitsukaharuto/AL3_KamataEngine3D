#pragma once
#include "Vector3.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Model.h"


class Collider {
public:
	Collider() = default;
	virtual ~Collider() = default;

	virtual void OnCollision([[maybe_unused]]Collider* other) {}

	virtual Vector3 GetCenterPosition() const = 0;

	virtual void Initialize();

	void UpdateWorldTransform();

	void Draw(Model* model, const ViewProjection& viewProjection);

	float GetRadius();

	void SetRadius();

	uint32_t GetTypeID() const;

	void SetTypeID(uint32_t typeID);

private:

	float radius_ = 1.5f;
	WorldTransform worldTransform;

	// 種別ID
	uint32_t typeID_ = 0u;

};