#include "Collider.h"

void Collider::Initialize() { 

	worldTransform.Initialize();

}

void Collider::UpdateWorldTransform() {

	worldTransform.translation_ = GetCenterPosition();
	worldTransform.UpdateMatrix();

}

void Collider::Draw(Model* model, const ViewProjection& viewProjection) {

	model->Draw(worldTransform, viewProjection);

}

float Collider::GetRadius() { return radius_; }