#include "Collider.h"

void Collider::Initialize() { 

	worldTransform.Initialize();

}

void Collider::UpdateWorldTransform() {

	worldTransform.translation_ = GetCenterPosition();
	worldTransform.scale_ = {radius_, radius_, radius_};
	worldTransform.UpdateMatrix();

}

void Collider::Draw(Model* model, const ViewProjection& viewProjection) {

	model->Draw(worldTransform, viewProjection);

}

float Collider::GetRadius() { return radius_; }

void Collider::SetRadius(const float rad) {

	radius_ = rad;

}

uint32_t Collider::GetTypeID() const { return typeID_; }

void Collider::SetTypeID(uint32_t typeID) { typeID_ = typeID; }
