#pragma once
#include "Vector3.h"


class Collider {
public:
	Collider() = default;
	virtual ~Collider() = default;

	virtual void OnCollision(){}

	virtual Vector3 GetCenterPosition() const = 0;

	float GetRadius();

	void SetRadius();

private:

	float radius_ = 1.5f;

};