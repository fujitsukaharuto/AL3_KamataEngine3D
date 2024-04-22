#pragma once
#include "Vector3.h"

class Collider {
public:

	float GetRadius();
	void SetRadius(float r);

	virtual void OnCollision();

	//日本語

	virtual Vector3 GetWorldPosition();

private:
	float radius_ = 1.5f;

};
