#include "Enemy.h"
#include "ImGuiManager.h"
#include "MathCal.h"

Enemy::Enemy() {}

Enemy::~Enemy() {}

void Enemy::Initialize(const std::vector<Model*>& models)
{

	BaseCharacter::Initialize(models);
	worldTransformBody_.Initialize();
	worldTransformL_arm_.Initialize();
	worldTransformR_arm_.Initialize();

	worldTransformBody_.parent_ = &worldTransform_;
	worldTransformL_arm_.parent_ = &worldTransformBody_;
	worldTransformR_arm_.parent_ = &worldTransformBody_;

	worldTransform_.translation_ = {5.0f, 0.0f, 0.0f};
	worldTransformL_arm_.translation_ = {-0.874f, 0.455f, 0.0f};
	worldTransformR_arm_.translation_ = {0.874f, 0.455f, 0.0f};
}

void Enemy::Update()
{
#ifdef _DEBUG

	ImGui::Begin("Enemy");
	ImGui::DragFloat3("Enemy L_arm", &worldTransformL_arm_.translation_.x, 0.01f);
	ImGui::DragFloat3("Enemy R_arm", &worldTransformR_arm_.translation_.x, 0.01f);
	ImGui::End();

#endif // _DEBUG

	Move();

	BaseCharacter::Update();
	worldTransformBody_.UpdateMatrix();
	worldTransformL_arm_.UpdateMatrix();
	worldTransformR_arm_.UpdateMatrix();

}

void Enemy::Draw(const ViewProjection& viewProjection)
{

	models_[0]->Draw(worldTransformBody_, viewProjection);
	models_[1]->Draw(worldTransformL_arm_, viewProjection);
	models_[1]->Draw(worldTransformR_arm_, viewProjection);

}

void Enemy::Move()
{

	float kCharacterSpeed = 0.2f;
	Vector3 velocity = {0.0f, 0.0f, -kCharacterSpeed};
	velocity = TransformNormal(velocity, worldTransform_.matWorld_);
	worldTransform_.translation_ += velocity;
	worldTransform_.rotation_.y += 0.05f;
}
