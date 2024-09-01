#include "StandbyOperation.h"
#include "MathCal.h"

StandbyOperation::StandbyOperation() {}

StandbyOperation::~StandbyOperation() {

	delete model_;

}

void StandbyOperation::Initialize(const Vector3& position, const int type) {

	if (type==0) {
		model_ = Model::Create();
	} else {
		model_ = Model::CreateSphere();
	}

	worldTransform_.Initialize();
	worldTransform_.scale_.y = 0.05f;
	worldTransform_.translation_ = position;

	shadow_.Initialize();

	worldTransform_.UpdateMatrix();

	color_.Initialize();
	color_.SetColor({1.0f, 0.0f, 0.0f, 0.75f});
	color_.TransferMatrix();
}

void StandbyOperation::TypeInitialize(const int type) {

	if (type == 0) {
		type_ = type;
		worldTransform_.scale_.z = 27.0f;

		worldTransform_.UpdateMatrix();

	} else {
		type_ = type;

		worldTransform_.scale_ = {2.0f, 0.05f, 2.0f};
		worldTransform_.rotation_ = {0.0f, 0.0f, 0.0f};

		worldTransform_.UpdateMatrix();
	}
}

void StandbyOperation::Update() {

	worldTransform_.UpdateMatrix();
	shadow_.UpdateMatrix();

	shadow_.matWorld_ = Multiply(worldTransform_.matWorld_,MakeIdentity4x4());

	shadow_.TransferMatrix();

	time_++;
	float frequency = 0.3f;
	float alpha = (sin(time_ * frequency) + 1.0f) / 2.0f;
	model_->SetAlpha(alpha);
}

void StandbyOperation::Draw(const ViewProjection& viewProjection) {

	model_->Draw(shadow_, viewProjection, &color_);

}

void StandbyOperation::SetTransform(const Vector3& trans) {

	worldTransform_.translation_ = trans;

}

void StandbyOperation::SetRotate(const Vector3& rotate) {

	worldTransform_.rotation_ = rotate;

}

