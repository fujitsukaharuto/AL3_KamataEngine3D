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

	for (EnemyAttackZone* attackZone : attackZones_) {
		delete attackZone;
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

	attackZones_.remove_if([](EnemyAttackZone* zone) {
		if (zone->IsDisappear()) {
			delete zone;
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

	Attack();

	for (LittleEnemy* littleEnemy : littleEnemys_) {
		littleEnemy->Update();
	}

	for (EnemyAttackZone* attackZone : attackZones_) {
		attackZone->Update();
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
	for (EnemyAttackZone* attackZone : attackZones_) {
		attackZone->Draw(viewProjection);
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

void Enemy::Attack() {

	if (attackZones_.size() == 0) {
		attackCooltime_--;
		if (attackCooltime_ <= 0) {
			float zoneRotate = 0.0f;

			for (int i = 0; i < 6; i++) {
				float interval = 6.0f;
				for (int j = 0; j < 5; j++) {

					EnemyAttackZone* newAttackZone = new EnemyAttackZone();
					Vector3 newPos = {0.0f, 0.0f, interval};
					interval += 6.0f;
					Matrix4x4 rotate = MakeRotateYMatrix(zoneRotate);
					newPos = TransformNormal(newPos, rotate);
					newPos += worldTransform_.translation_;

					newAttackZone->Initialize(models_[1], newPos);
					attackZones_.push_back(newAttackZone);
				}
				zoneRotate += 0.785398f;
			}

			for (int i = 0; i < 6; i++) {
				EnemyAttackZone* newAttackZone = new EnemyAttackZone();
				Vector3 newPos = {0.0f, 0.0f, 6.0f};
				Matrix4x4 rotate = MakeRotateYMatrix(zoneRotate);
				newPos = TransformNormal(newPos, rotate);
				newPos += worldTransform_.translation_;

				newAttackZone->Initialize(models_[1], newPos);

				zoneRotate += 1.0472f;
				attackZones_.push_back(newAttackZone);
			}
			zoneRotate = 0.0f;
			for (int i = 0; i < 12; i++) {
				EnemyAttackZone* newAttackZone = new EnemyAttackZone();
				Vector3 newPos = {0.0f, 0.0f, 15.0f};
				Matrix4x4 rotate = MakeRotateYMatrix(zoneRotate);
				newPos = TransformNormal(newPos, rotate);
				newPos += worldTransform_.translation_;

				newAttackZone->Initialize(models_[1], newPos);

				zoneRotate += 0.523599f;
				attackZones_.push_back(newAttackZone);
			}
			attackCooltime_ = 60;
		}
	}
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

void Enemy::SetLittleEnemyTarget(const Vector3& target) {
	for (LittleEnemy* littleEnemy : littleEnemys_) {
		littleEnemy->SetTargetPosision(target);
	}
}

std::list<LittleEnemy*> Enemy::GetLittleEnemyCollider() { return littleEnemys_; }

std::list<EnemyAttackZone*> Enemy::GetAttackZoneCollider() { return attackZones_; }
