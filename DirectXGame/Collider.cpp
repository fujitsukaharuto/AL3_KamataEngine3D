#include "Collider.h"

float Collider::GetRadius() { return radius_; }

void Collider::SetRadius(float r)
{

	radius_ = r;

}

void Collider::OnCollision() {}

Vector3 Collider::GetWorldPosition() { return Vector3(); }
