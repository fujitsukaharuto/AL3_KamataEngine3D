#pragma once
#include "Vector3.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Model.h"


class Collider {
public:
	Collider() = default;
	virtual ~Collider() = default;

	virtual void OnCollision(){}

	virtual Vector3 GetCenterPosition() const = 0;

	void Initialize();

	void UpdateWorldTransform();

	void Draw(Model* model, const ViewProjection& viewProjection);

	float GetRadius();

	void SetRadius();

private:

	float radius_ = 1.5f;
	WorldTransform worldTransform;

};