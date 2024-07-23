#include "EnemyHitEffect.h"
#include "MathCal.h"

void EnemyHitEffect::Initialize() {

	model_.reset(Model::CreateSphere());
	worldTransform_.Initialize();
	worldTransform_.UpdateMatrix();

}

void EnemyHitEffect::UpdateWorldTransform() {

	float t = (10.0f - timer_) * 0.1f;
	float scale = Lerp(1.0f, 3.0f, (t));

	worldTransform_.scale_ = {scale, scale, scale};

	float alpha = 1.0f;
	alpha = 1.0f - Lerp(1.0f, 0.0f, timer_ / 10.0f);
	model_->SetAlpha(alpha);

	worldTransform_.UpdateMatrix();
	timer_--;
	if (timer_ < 0) {
		isDeath_ = true;
	}
}

void EnemyHitEffect::Draw(const ViewProjection& viewProjection) {

	model_->Draw(worldTransform_, viewProjection);

}

void EnemyHitEffect::SetPosition(const Vector3& translation) {

	worldTransform_.translation_ = translation;

}

void EnemyHitEffect::SetParent(const WorldTransform& worldTrans) {

	worldTransform_.parent_ = &worldTrans;

}

bool EnemyHitEffect::GetIsDeath() const { return isDeath_; }
