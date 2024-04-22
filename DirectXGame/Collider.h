#pragma once
#include "Vector3.h"
#include "CollisionConfig.h"

class Collider {
public:

	float GetRadius();
	void SetRadius(float r);

	virtual void OnCollision();

	//日本語

	virtual Vector3 GetWorldPosition();

	uint32_t GetAttribute() { return collisionAttribute_; }
	void SetAttribute(uint32_t attri) { collisionAttribute_ = attri; }

	uint32_t GetMask() { return CollisionMask_; }
	void SetMask(uint32_t mask) { CollisionMask_ = mask; }

private:
	float radius_ = 1.5f;
	uint32_t collisionAttribute_ = 0xffffffff;
	uint32_t CollisionMask_ = 0xffffffff;

};
