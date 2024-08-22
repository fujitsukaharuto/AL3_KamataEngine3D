#include "Enemy.h"
#include "ImGuiManager.h"
#include "MathCal.h"
#include "CollisionTypeIdDef.h"

uint32_t Enemy::nextNerialNumber_ = 0;

Enemy::Enemy() {

	serialNumber_ = nextNerialNumber_;
	++nextNerialNumber_;
}

Enemy::~Enemy() {

	for (LittleEnemy* littleEnemy : littleEnemys_) {
		delete littleEnemy;
	}
}

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

	Collider::SetTypeID(static_cast<uint32_t>(CollisionTypeIdDef::kEnemy));

	float newPos = 2.0f;
	for (int i = 0; i < 10; i++) {
		LittleEnemy* newLittleEnemy = new LittleEnemy();

		Vector3 newLittleEPos = {newPos, 0.0f, newPos * 0.5f};
		newPos += 2.0f;

		newLittleEnemy->Initialize(models_);
		newLittleEnemy->SetPosition(newLittleEPos);

		littleEnemys_.push_back(newLittleEnemy);
	}

}

void Enemy::Update()
{

	littleEnemys_.remove_if([](LittleEnemy* littleE) {
		if (littleE->IsDead()) {
			delete littleE;
			return true;
		}
		return false;
	});

#ifdef _DEBUG

	ImGui::Begin("Enemy");
	ImGui::DragFloat3("Enemy L_arm", &worldTransformL_arm_.translation_.x, 0.01f);
	ImGui::DragFloat3("Enemy R_arm", &worldTransformR_arm_.translation_.x, 0.01f);
	ImGui::End();

#endif // _DEBUG

	Move();
	UpdatePartsGimmick();

	for (LittleEnemy* littleEnemy : littleEnemys_) {
		littleEnemy->Update();
	}

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

	for (LittleEnemy* littleEnemy : littleEnemys_) {
		littleEnemy->Draw(viewProjection);
	}

}

void Enemy::Move()
{
	float mpi = 3.14159265f;
	float kCharacterSpeed = 0.05f;
	Vector3 velocity = {0.0f, 0.0f, -kCharacterSpeed};
	velocity = TransformNormal(velocity, worldTransform_.matWorld_);
	worldTransform_.translation_ += velocity;
	worldTransform_.rotation_.y += 0.01f;
	worldTransform_.rotation_.y = std::fmod(worldTransform_.rotation_.y, 2.0f * mpi);
}

void Enemy::UpdatePartsGimmick()
{
	float mpi = 3.14159265f;
	float kPartsRotateSpeed = 0.2f;
	worldTransformL_arm_.rotation_.x += -kPartsRotateSpeed;
	worldTransformR_arm_.rotation_.x += -kPartsRotateSpeed;
	worldTransformL_arm_.rotation_.x = std::fmod(worldTransformL_arm_.rotation_.x, 2.0f * mpi);
	worldTransformR_arm_.rotation_.x = std::fmod(worldTransformR_arm_.rotation_.x, 2.0f * mpi);
}

void Enemy::OnCollision([[maybe_unused]] Collider* other) {}

Vector3 Enemy::GetCenterPosition() const {

	const Vector3 offset = {0.0f, 1.0f, 0.0f};

	Vector3 worldPos = Transform(offset, worldTransformBody_.matWorld_);

	return worldPos;
}

uint32_t Enemy::GetSerialNumber() const { return serialNumber_; }

void Enemy::SettingLittles() {

	if (littleEnemys_.size() == 0) {
		float newPos = 2.0f;
		for (int i = 0; i < 10; i++) {
			LittleEnemy* newLittleEnemy = new LittleEnemy();

			Vector3 newLittleEPos = {newPos, 0.0f, newPos * 0.5f};
			newPos += 2.0f;

			newLittleEnemy->Initialize(models_);
			newLittleEnemy->SetPosition(newLittleEPos);

			littleEnemys_.push_back(newLittleEnemy);
		}
	}

}

std::list<LittleEnemy*> Enemy::GetLittleEnemyCollider() { return littleEnemys_; }
