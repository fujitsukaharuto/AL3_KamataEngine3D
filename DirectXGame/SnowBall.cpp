#include "SnowBall.h"
#include "MathCal.h"
#include "LittleEnemy.h"
#include "ImGuiManager.h"
#include <cassert>

SnowBall::SnowBall() {}

SnowBall::~SnowBall() {

	for (LittleEnemy* littleE : littleEnemys_) {

		delete littleE;
	}

}

void SnowBall::Initialize(Model* model, const Vector3& position) {
	assert(model);
	model_ = model;

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;

	worldTransform_.scale_ = {sizeRadius_, sizeRadius_, sizeRadius_};

	worldTransform_.UpdateMatrix();

	Collider::Initialize();
	Collider::SetTypeID(static_cast<uint32_t>(CollisionTypeIdDef::kSnowBall));
	Collider::SetRadius(sizeRadius_);
}

void SnowBall::Update() {

	littleEnemys_.remove_if([](LittleEnemy* littleE) {
		if (littleE->IsDead()) {
			delete littleE;
			return true;
		}
		return false;
	});

	if (isRemove_) {
	
		worldTransform_.translation_ += velocity_;
		worldTransform_.scale_ = {sizeRadius_, sizeRadius_, sizeRadius_};

		Vector3 Differential = velocity_;
		Differential = Differential.Normalize();

		Vector3 forward = {0.0f, 1.0f, 0.0f};
		Vector3 rotateAxis = forward.Cross(Differential);
		if (rotateAxis.Lenght() > 0.0f) {

			rotateAxis = rotateAxis.Normalize();

			float rotateAngle = (Differential.Lenght() / sizeRadius_);

			Matrix4x4 rotateMat = MakeRotateAxisMatrix(rotateAxis, rotateAngle);

			Matrix4x4 currentTransform = worldTransform_.matWorld_;

			currentTransform = Multiply(rotateMat, currentTransform);

			Vector3 newRotate = ExtractEulerAngles(currentTransform);

			worldTransform_.rotation_ = newRotate;
			ImGui::Begin("size");
			ImGui::Text("%f", rotateAngle);
			ImGui::Text("%f", worldTransform_.rotation_.x);
			ImGui::Text("%f", worldTransform_.rotation_.y);
			ImGui::Text("%f", worldTransform_.rotation_.z);
			ImGui::End();
		}

		worldTransform_.UpdateMatrix();
		deathTimer_--;
		if (deathTimer_<=0) {
		
			isDead_ = true;
		
		}
	} else {
	
		worldTransform_.scale_ = {sizeRadius_, sizeRadius_, sizeRadius_};
		worldTransform_.UpdateMatrix();
		Collider::SetRadius(sizeRadius_);
	}

	for (LittleEnemy* littleE : littleEnemys_) {

		littleE->Update();
		littleE->SetSclae(littleE->GetOrigineScale() / sizeRadius_);
	}

}

void SnowBall::Draw(const ViewProjection& viewProjection) {

	model_->Draw(worldTransform_, viewProjection);
	for (LittleEnemy* littleE : littleEnemys_) {

		littleE->Draw(viewProjection);
	}

}

Vector3 SnowBall::GetCenterPosition() const { 

	const Vector3 offset = {0.0f, 0.0f, 0.0f};

	Vector3 worldPos = Transform(offset, worldTransform_.matWorld_);

	return worldPos;
}

float SnowBall::GetRadius() { return sizeRadius_; }

Vector3 SnowBall::GetTrans() { return worldTransform_.translation_; }

Matrix4x4 SnowBall::GetWorldMat() { return worldTransform_.matWorld_; }

void SnowBall::OnCollision(Collider* other) {

	uint32_t typeID = other->GetTypeID();
	// 衝突相手が敵なら
	if (typeID == static_cast<uint32_t>(CollisionTypeIdDef::kEnemy)) {
		isDead_ = true;
	}
	if (typeID == static_cast<uint32_t>(CollisionTypeIdDef::kLittleEnemy)) {
		littleEnemyCount_++;
		sizeRadius_ += addRadius_;
		worldTransform_.translation_.y += addRadius_;
		Collider::SetRadius(sizeRadius_);


		for (LittleEnemy* littleE : littleEnemys_) {

			littleE->SetSclae(littleE->GetOrigineScale() / sizeRadius_);
		}


		LittleEnemy* newLittle = new LittleEnemy();
		newLittle->Initialize(littleModel_);
		newLittle->SetIsGetCaught(true);
		Matrix4x4 invRotate = MakeRotateXYZMatrix((worldTransform_.rotation_));
		invRotate = Inverse(invRotate);
		Vector3 invrad = TransformNormal({0.0f, -1.0f, 0.0f}, invRotate);
		newLittle->SetRotate(invrad);
		newLittle->SetPosition(invrad);
		newLittle->Setparent(worldTransform_);
		newLittle->SetSclae(newLittle->GetOrigineScale() / sizeRadius_);
		newLittle->Update();
		littleEnemys_.push_back(newLittle);
	}

}

void SnowBall::SetTransform(const Vector3& trans) {

	worldTransform_.translation_ = trans;

}

void SnowBall::SetVelocity(const Vector3& velo) {

	velocity_ = velo;

}

void SnowBall::SetRemove(bool isRe) {

	isRemove_ = isRe;

}

void SnowBall::SetRotate(const Vector3& rotate) { worldTransform_.rotation_ = rotate; }

void SnowBall::SetSizeRadius(const float rad) {

	sizeRadius_ = rad;

}

void SnowBall::SetLittleModel(std::vector<Model*> model) {

	littleModel_ = model;

}
